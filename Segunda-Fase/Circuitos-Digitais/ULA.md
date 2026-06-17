# Guia completo — ULA do Grupo 30 no Logisim Evolution

> **Escopo desta entrega (17/06):** aqui montamos **só a ULA**. O banco de
> registradores e a integração final ficam pra entrega de 01/07. Pra testar a
> ULA sozinha, as entradas `A` e `B` virão de chaves diretamente (depois elas
> serão substituídas pelas saídas `VA` e `VB` do banco de registradores).

> **Premissa importante:** este guia assume que você **nunca usou Logisim**.
> Cada bloco tem: o que é, a tabela-verdade (que também vai no relatório), a
> equação pronta pra colar, e o passo-a-passo de montagem na tela.

---

## Sumário

1. [O que a ULA do Grupo 30 faz](#1)
2. [Logisim 101 — operando a ferramenta](#2)
3. [Como "importar" expressões (Análise Combinacional)](#3)
4. [Estrutura de subcircuitos que vamos criar](#4)
5. [Bloco A — Operações lógicas (NAND, AND, OR, XOR)](#5)
6. [Bloco B — Full Adder (somador de 1 bit)](#6)
7. [Bloco C — Somador de 8 bits](#7)
8. [Bloco D — Subtrator (reuso do somador)](#8)
9. [Bloco E — Multiplicador e Divisor](#9)
10. [Bloco F — Decodificador 3→8](#10)
11. [Bloco G — Multiplexador 8:1 de dados](#11)
12. [Bloco H — Flag Z (zero)](#12)
13. [Bloco I — Flag C (overflow)](#13)
14. [Montagem final (top-level)](#14)
15. [Roteiro de teste](#15)
16. [Checklist de entrega + o que vai no relatório](#16)
17. [Tabela-resumo de expressões pra colar](#17)

---

<a name="1"></a>
## 1. O que a ULA do Grupo 30 faz

A ULA recebe dois números de 8 bits (`A` e `B`) e um seletor de 3 bits
(`SelOp`). Conforme o `SelOp`, ela entrega na saída `S` o resultado de **uma**
das 8 operações abaixo. Ela também acende dois LEDs: `Z` (resultado é zero) e
`C` (houve overflow/underflow).

| SelOp | Operação  | Tipo        |
|-------|-----------|-------------|
| 000   | A NAND B  | lógica      |
| 001   | A − B     | aritmética  |
| 010   | A × B     | aritmética  |
| 011   | A OR B    | lógica      |
| 100   | A AND B   | lógica      |
| 101   | A ÷ B     | aritmética  |
| 110   | A XOR B   | lógica      |
| 111   | A + B     | aritmética  |

**Ideia central da arquitetura:** calculamos **as 8 operações ao mesmo tempo**
(em paralelo) sobre `A` e `B`, e um **multiplexador** escolhe qual resultado sai
em `S`, usando o `SelOp` como chave seletora.

```
              A(8)   B(8)
                │      │
     ┌──────────┼──────┼──────────────┐
     ▼          ▼      ▼               ▼
  [NAND]     [SUB]  [MULT] ... [SOMA]  (8 blocos, todos 8 bits)
     │ ent0     │ent1  │ent2  ...  │ent7
     └────────┬─┴──────┴───────────┘
              ▼
        MUX 8:1 (8 bits) ◄── SelOp(3)
              │
              ▼
            S(8) ──► flag Z
                  ──► flag C  (mux separado, mesmo SelOp)
```

---

<a name="2"></a>
## 2. Logisim 101 — operando a ferramenta

### Instalar
Baixe o **Logisim Evolution 3.9 ou superior** (exigência do enunciado) em:
`https://github.com/logisim-evolution/logisim-evolution` → seção *Releases*.
É um `.jar` (precisa de Java instalado) ou um instalador conforme o seu sistema.

### A tela (4 áreas)
- **Barra de ferramentas / Explorer (esquerda):** lista de bibliotecas
  (Wiring, Gates, Plexers, Arithmetic, Memory…) e os seus subcircuitos.
- **Canvas (centro):** onde você desenha o circuito.
- **Atributos (canto inferior esquerdo):** propriedades do componente
  selecionado (largura em bits, nº de entradas, rótulo, etc.).
- **Aba de simulação:** o "dedo" (Poke tool) liga/desliga entradas.

### Os 4 verbos que você vai repetir o tempo todo
1. **Colocar um componente:** clique nele no Explorer, depois clique no canvas.
2. **Ligar fios:** selecione a **ferramenta de edição** (a setinha / "Edit
   tool"). Passe o mouse sobre um pino (aparece um circulinho verde), clique e
   arraste até o outro pino. Fios são ortogonais (só vertical/horizontal).
3. **Mudar largura em bits:** selecione o componente → no painel de Atributos
   mude **"Data Bits"** (ou "Bit Width") para `8`. **Isso é o que torna uma
   porta "bit a bit" de 8 bits.**
4. **Simular:** selecione a **ferramenta Poke** (mãozinha) e clique nas entradas
   pra alternar valores. Verde claro = 1, verde escuro = 0, vermelho = erro
   (geralmente curto ou largura incompatível), cinza/azul = indefinido.

### 4 componentes de "fiação" que vão te salvar
Estão na biblioteca **Wiring**:

- **Pin (Input Pin / Output Pin):** as bordas do circuito. Input = entrada
  (você seta o valor); Output = saída (LED ou pino). Tem atributo de largura.
- **Splitter (divisor):** pega um barramento de 8 bits e separa em fios
  individuais (ou junta). **Essencial** — você vai usar muito pra pegar bits
  individuais de `A`, `B` e `S`.
- **Tunnel (túnel):** um "fio sem fio". Dois túneis com o **mesmo nome** estão
  eletricamente conectados, mesmo sem linha entre eles. Use pra não cruzar
  espaguete de fios (ex: leve `SelOp` pra vários blocos com túneis chamados
  `s0`, `s1`, `s2`).
- **Bit Extender (extensor):** transforma N bits em M bits. Vamos usar pra
  "esticar" 1 bit em 8 bits.

### Subcircuitos (a parte mais importante pra organização)
Um subcircuito é um "chip" que você cria e reusa. Você desenha o Full Adder
**uma vez**, e ele vira um bloco que você coloca 8 vezes no somador.

- **Criar:** menu **Project → Add Circuit…** → dê um nome (ex: `FullAdder`).
  Ele aparece no Explorer e você pode editá-lo.
- **Usar:** clique no nome dele no Explorer e clique no canvas de outro
  circuito. Os Input/Output Pins viram os pinos externos do chip.
- O circuito **principal** (top-level) costuma se chamar `main`.

---

<a name="3"></a>
## 3. Como "importar" expressões — Análise Combinacional

Esse é o recurso que você quer: digitar uma equação booleana e o Logisim
**desenha o circuito sozinho**. Funciona pra blocos pequenos (poucas entradas).
Vamos usar no **Full Adder** e no **Decodificador**.

**Passo a passo:**
1. Crie/abra o subcircuito onde o circuito gerado vai entrar (ex: `FullAdder`).
2. Menu **Window → Combinational Analysis** (em algumas versões: *Project →
   Analyze Circuit*, mas pra construir do zero use a janela do menu Window).
3. **Aba "Inputs":** adicione as variáveis de entrada (ex: `a`, `b`, `cin`).
4. **Aba "Outputs":** adicione as saídas (ex: `s`, `cout`).
5. **Aba "Expression":** escolha uma saída no topo, digite a expressão no campo
   e clique **"Enter"**. Repita pra cada saída.
6. Clique em **"Build Circuit"** → escolha o circuito de destino → ele gera as
   portas e fios automaticamente.

**Sintaxe das expressões no Logisim:**

| Operação | Como escrever | Exemplo |
|----------|---------------|---------|
| NOT      | `~` antes da variável | `~a` |
| AND      | espaço entre variáveis (justaposição) ou `&` | `a b` ou `a & b` |
| OR       | `+` | `a + b` |
| XOR      | `^` | `a ^ b` |

Precedência: NOT > AND > XOR > OR. Use parênteses na dúvida.

> **Dica de iniciante:** se errar de digitar uma expressão é chato de debugar.
> A janela também tem a **aba "Table" (tabela-verdade)** — você pode simplesmente
> **marcar os 0/1 na tabela** em vez de digitar a expressão, e mesmo assim clicar
> "Build Circuit". Dou as duas formas (tabela + expressão) em cada bloco.

---

<a name="4"></a>
## 4. Estrutura de subcircuitos que vamos criar

Crie estes subcircuitos (Project → Add Circuit). Vamos preenchê-los um a um:

| Subcircuito     | Para quê |
|-----------------|----------|
| `FullAdder`     | somador de 1 bit (gerado por expressão) |
| `Somador8`      | 8 × FullAdder em cascata |
| `Decoder3x8`    | transforma SelOp em 8 linhas one-hot |
| `main`          | a ULA inteira montada (já existe por padrão) |

As operações lógicas, multiplicador, divisor, MUX e flags podem morar direto no
`main` (são simples) — mas se quiser organizar, nada impede criar subcircuitos
pra elas também.

---

<a name="5"></a>
## 5. Bloco A — Operações lógicas (NAND, AND, OR, XOR)

**São as mais fáceis.** Como são bit a bit, basta **uma porta de 8 bits** pra
cada operação. Nada de fazer 8 portas.

**Montagem (faça pra cada uma das 4):**
1. Biblioteca **Gates** → coloque a porta desejada (NAND, AND, OR, XOR).
2. Selecione-a → em Atributos mude **Data Bits = 8**.
3. Ligue `A` na entrada de cima e `B` na de baixo.
4. A saída de 8 bits é o resultado daquela operação. Leve com um túnel:
   `NAND → túnel "op_nand"`, `OR → "op_or"`, `AND → "op_and"`, `XOR → "op_xor"`.

**Equações (bit a bit, para cada bit i de 0 a 7):**

```
NAND:  Si = ~(Ai & Bi)
AND:   Si =   Ai & Bi
OR:    Si =   Ai + Bi
XOR:   Si =   Ai ^ Bi
```

**Flag C dessas operações = 0** (lógicas não geram overflow).

---

<a name="6"></a>
## 6. Bloco B — Full Adder (somador de 1 bit)

É o tijolo do somador. Soma 3 bits: `a`, `b` e o "vai-um" de entrada `cin`.
Produz a soma `s` e o "vai-um" de saída `cout`.

**Tabela-verdade (vai no relatório):**

| a | b | cin | s | cout |
|---|---|-----|---|------|
| 0 | 0 | 0   | 0 | 0    |
| 0 | 0 | 1   | 1 | 0    |
| 0 | 1 | 0   | 1 | 0    |
| 0 | 1 | 1   | 0 | 1    |
| 1 | 0 | 0   | 1 | 0    |
| 1 | 0 | 1   | 0 | 1    |
| 1 | 1 | 0   | 0 | 1    |
| 1 | 1 | 1   | 1 | 1    |

**Equações simplificadas (cole na Análise Combinacional):**

```
s    = a ^ b ^ cin
cout = a b + a cin + b cin
```

> A forma `a b + cin (a ^ b)` também é válida e usa menos portas, mas
> `a b + a cin + b cin` (função maioria) é mais fácil de não errar ao digitar.

**Montagem:**
1. `Project → Add Circuit → FullAdder`.
2. `Window → Combinational Analysis`.
3. Inputs: `a`, `b`, `cin`. Outputs: `s`, `cout`.
4. Aba Expression: selecione `s`, digite `a ^ b ^ cin`, **Enter**. Selecione
   `cout`, digite `a b + a cin + b cin`, **Enter**.
5. **Build Circuit** → destino `FullAdder`. Pronto, o chip está gerado.

---

<a name="7"></a>
## 7. Bloco C — Somador de 8 bits

Não dá pra gerar por expressão (são muitas entradas). Monta-se **estruturalmente**:
8 `FullAdder` em cascata, ligando o `cout` de um no `cin` do próximo
(*ripple-carry*).

**Montagem no subcircuito `Somador8`:**
1. `Project → Add Circuit → Somador8`.
2. Coloque **2 Input Pins de 8 bits** (`A` e `B`) e **1 Input Pin de 1 bit**
   (`Cin`). Coloque **1 Output Pin de 8 bits** (`Soma`) e Output Pins de 1 bit
   (`Cout`, e também `Cin7` e `Cout7` — explico já já).
3. Coloque **2 Splitters** configurados: *Bit Width In = 8*, *Fan Out = 8*.
   Ligue `A` num splitter e `B` no outro → agora você tem `a0..a7` e `b0..b7`.
4. Coloque **8 FullAdder** em fila.
5. Ligue, para cada bit i: `ai` → entrada a; `bi` → entrada b.
6. Cadeia de carry:
   - FullAdder do bit 0: `cin` = o pino `Cin` do circuito.
   - bit 1: `cin` = `cout` do bit 0.
   - … e assim por diante até o bit 7.
7. Junte as 8 saídas `s0..s7` em **outro Splitter** (8→1) pra formar `Soma` (8 bits).
8. `Cout` do circuito = `cout` do bit 7.
9. **Exponha também `Cin7` (o cin que entra no bit 7) e `Cout7` (o cout do bit
   7)** como saídas. Eles são necessários pra calcular o overflow com sinal
   depois.

**Por que expor Cin7 e Cout7?** No complemento de 2, o overflow com sinal é:
```
overflow = Cin(bit mais significativo) ^ Cout(bit mais significativo)
overflow = Cin7 ^ Cout7
```

---

<a name="8"></a>
## 8. Bloco D — Subtrator (A − B)

Em complemento de 2: `A − B = A + (~B) + 1`. Reusa o `Somador8`:

1. Coloque **um segundo `Somador8`** no `main`.
2. A entrada `A` dele recebe `A` normalmente.
3. A entrada `B` dele recebe `B` **passado por uma porta NOT de 8 bits**
   (inverte os 8 bits → vira `~B`).
4. A entrada `Cin` dele = **constante 1** (Wiring → Constant, valor 1). Esse é o
   "+1" do complemento de 2.
5. A saída `Soma` desse somador é o resultado de `A − B`. Leve com túnel `op_sub`.
6. Guarde o `overflow` dele: `Cin7 ^ Cout7` (uma porta XOR de 1 bit) → túnel `ov_sub`.

> Use **dois `Somador8` separados** (um pra soma, um pra subtração). É mais
> bobo de montar e impossível de confundir, o que é o que importa com o prazo.
> O somador da soma: `Cin = 0` (Constant 0), saída → túnel `op_soma`, overflow
> `Cin7 ^ Cout7` → túnel `ov_soma`.

---

<a name="9"></a>
## 9. Bloco E — Multiplicador e Divisor (módulos prontos)

O enunciado **permite** os módulos prontos aqui. Estão na biblioteca **Arithmetic**.

**Multiplicador (A × B):**
1. Coloque o **Multiplier**. Data Bits = 8.
2. Ligue `A` e `B`.
3. Ele dá um resultado de até **16 bits**, dividido em saída "baixa" (8 bits
   menos significativos) e uma saída de "carry"/parte alta. Confira no próprio
   componente qual é qual.
4. **`S` da multiplicação = os 8 bits baixos.** Túnel `op_mult`.
5. **Overflow `ov_mult`:** se a parte **alta** (bits 8–15) for diferente de
   zero, o resultado não coube em 8 bits → overflow. Pegue a parte alta, jogue
   num **OR de 8 bits de entrada** (após splitter) → `ov_mult`.

**Divisor (A ÷ B):**
1. Coloque o **Divider**. Data Bits = 8.
2. Ligue `A` no dividendo e `B` no divisor.
3. Ele tem saídas de **quociente** e **resto**.
4. **`S` da divisão = quociente.** Túnel `op_div`. (Documente essa escolha no
   relatório.)
5. **Overflow `ov_div`:** convenção usual = divisão por zero. `ov_div = (B == 0)`,
   que é o **NOR dos 8 bits de B** (B é zero quando todos os bits são 0).

---

<a name="10"></a>
## 10. Bloco F — Decodificador 3→8

Transforma `SelOp = s2 s1 s0` em 8 linhas `E0..E7`, onde **exatamente uma** fica
em 1 (a correspondente ao valor de SelOp) e o resto em 0. É o "cérebro" do MUX.

**Equações (cole na Análise Combinacional — `s2` é o bit mais significativo):**

```
E0 = ~s2 ~s1 ~s0
E1 = ~s2 ~s1  s0
E2 = ~s2  s1 ~s0
E3 = ~s2  s1  s0
E4 =  s2 ~s1 ~s0
E5 =  s2 ~s1  s0
E6 =  s2  s1 ~s0
E7 =  s2  s1  s0
```

**Montagem:**
1. `Project → Add Circuit → Decoder3x8`.
2. `Window → Combinational Analysis`. Inputs: `s2`, `s1`, `s0`. Outputs:
   `E0`…`E7`.
3. Cole cada expressão na aba Expression. **Build Circuit → Decoder3x8.**

> Pelo enunciado ("apenas portas lógicas…"), evite o componente **Decoder**
> pronto da biblioteca Plexers. Construa por portas como acima. (Se o professor
> liberar Plexers, é só largar o Decoder pronto e pular este passo — **confirme
> antes**.)

---

<a name="11"></a>
## 11. Bloco G — Multiplexador 8:1 de dados

Escolhe qual dos 8 resultados (cada um de 8 bits) vai pra `S`, usando as linhas
`E0..E7` do decodificador. Como só uma `Ek` é 1 por vez, a técnica é:
**mascarar cada resultado com sua linha e somar (OR) tudo.**

A ordem das entradas segue o `SelOp` do Grupo 30:

| Entrada | SelOp | Operação | Túnel do resultado |
|---------|-------|----------|--------------------|
| 0       | 000   | A NAND B | `op_nand` |
| 1       | 001   | A − B    | `op_sub`  |
| 2       | 010   | A × B    | `op_mult` |
| 3       | 011   | A OR B   | `op_or`   |
| 4       | 100   | A AND B  | `op_and`  |
| 5       | 101   | A ÷ B    | `op_div`  |
| 6       | 110   | A XOR B  | `op_xor`  |
| 7       | 111   | A + B    | `op_soma` |

**Montagem (repita pra cada um dos 8 blocos k = 0..7):**
1. Coloque um **Bit Extender** (Wiring): *Bit Width In = 1*, *Bit Width Out = 8*,
   *Type = Sign*. Entrada = linha `Ek`. Saída = `0xFF` se `Ek=1`, senão `0x00`.
   (Isso "espalha" o 1 bit de seleção pros 8 bits.)
2. Coloque uma **porta AND de 8 bits, 2 entradas**. Entradas: o resultado do
   bloco k (ex: `op_nand`) e a máscara do Bit Extender. Saída = resultado se
   selecionado, senão `0x00`.
3. Depois de fazer os 8, coloque **uma porta OR de 8 bits com 8 entradas**.
   Ligue as 8 saídas AND nela. **A saída dessa OR é `S` (8 bits).**
4. Leve `S` num túnel `S`.

```
Ek (1 bit) ─► [Bit Extender 1→8, Sign] ─┐
                                         ▼
op_k (8 bits) ───────────────────► [AND 8b] ─► (mascarado)
   ... (8 vezes) ...                          └──► [OR 8b, 8 entradas] ─► S
```

> **Atalho (só se Plexers for permitido):** apague tudo isso e coloque um
> **Multiplexer** pronto: *Select Bits = 3*, *Data Bits = 8*. Ligue os 8
> resultados nas 8 entradas (na ordem da tabela acima) e `SelOp` no Select.
> Saída = `S`. **Confirme com o professor antes de usar.**

---

<a name="12"></a>
## 12. Bloco H — Flag Z (zero)

`Z = 1` quando `S` é totalmente zero. (Item 7 do enunciado: **todas** as
operações afetam Z, então calcule a partir do `S` **final**, depois do mux.)

```
Z = ~(S0 + S1 + S2 + S3 + S4 + S5 + S6 + S7)
```

**Montagem:**
1. **Splitter** em `S` (8→1) pra obter `S0..S7`.
2. **Porta NOR de 1 bit com 8 entradas** → ligue os 8 bits.
3. Saída = `Z` → Output Pin de 1 bit com LED.

---

<a name="13"></a>
## 13. Bloco I — Flag C (overflow/underflow)

Só as **4 operações aritméticas** afetam C. Usamos um **segundo mux 1:8**
(reaproveitando o mesmo decodificador) pra escolher a fonte de overflow conforme
o `SelOp`. Nas operações lógicas, a entrada é **0** (Constant 0).

| Entrada | SelOp | Operação | Fonte de C |
|---------|-------|----------|------------|
| 0 | 000 | NAND  | `0`        |
| 1 | 001 | A − B  | `ov_sub`  |
| 2 | 010 | A × B  | `ov_mult` |
| 3 | 011 | OR     | `0`        |
| 4 | 100 | AND    | `0`        |
| 5 | 101 | A ÷ B  | `ov_div`  |
| 6 | 110 | XOR    | `0`        |
| 7 | 111 | A + B  | `ov_soma` |

Como só `E1, E2, E5, E7` importam (o resto multiplica por 0), dá pra simplificar
num único bloco de portas:

```
C = E1 ov_sub + E2 ov_mult + E5 ov_div + E7 ov_soma
```

**Montagem (forma simples por portas):**
1. 4 portas **AND de 1 bit, 2 entradas**:
   `E1 & ov_sub`, `E2 & ov_mult`, `E5 & ov_div`, `E7 & ov_soma`.
2. 1 porta **OR de 1 bit, 4 entradas** ligando as 4 saídas AND.
3. Saída = `C` → Output Pin de 1 bit com LED.

> Onde vêm os `ov_*`: `ov_sub` e `ov_soma` = `Cin7 ^ Cout7` de cada somador
> (Bloco D); `ov_mult` = parte alta do produto ≠ 0 (Bloco E); `ov_div` = `B==0`
> (Bloco E). Critérios de mult/div são escolha de projeto — **explique no
> relatório**.

---

<a name="14"></a>
## 14. Montagem final (top-level `main`)

Agora junte tudo no circuito `main`:

**Entradas (com chaves, conforme item 1 do enunciado):**
- `A`: Input Pin de **8 bits**. (Dica: use um **DipSwitch** de 8 bits da
  biblioteca Input/Output pra ter "chavinhas" físicas, ou um Input Pin e edite
  o valor com o Poke.)
- `B`: Input Pin de **8 bits**.
- `SelOp`: Input Pin de **3 bits** → ligue num Splitter (3→1) pra gerar
  `s2, s1, s0` e leve com túneis. (Ou 3 chaves de 1 bit.)

**Saídas (com LEDs, item 2 do enunciado):**
- `S`: Output Pin de **8 bits**. Opcional: ligue 8 LEDs ou um Hex Display.
- `Z`: Output Pin de 1 bit + LED.
- `C`: Output Pin de 1 bit + LED.

**Ligações (ordem recomendada):**
1. Leve `A` e `B` (via túneis `A`, `B`) pra **todos** os blocos:
   - portas lógicas (NAND/AND/OR/XOR de 8 bits),
   - os dois `Somador8` (soma e subtração),
   - Multiplier e Divider.
2. Leve `SelOp` pro `Decoder3x8` → gera `E0..E7` (túneis).
3. Cada bloco produz seu túnel `op_*` e (se aritmético) seu `ov_*`.
4. Mux de dados (Bloco G) → `S`.
5. Flag Z (Bloco H) a partir de `S`.
6. Flag C (Bloco I) a partir das `E*` e `ov_*`.

**Dica anti-bug:** use **túneis nomeados** em vez de fios longos. Crie túneis
`A`, `B`, `s0`, `s1`, `s2`, `E0`…`E7`, `op_nand`…`op_soma`, `ov_sub`,
`ov_soma`, `ov_mult`, `ov_div`, `S`. Fica limpo e fácil de achar erro.

---

<a name="15"></a>
## 15. Roteiro de teste (faça antes de gravar o vídeo)

Use a ferramenta **Poke** (mãozinha). Teste **cada SelOp** com valores que você
consegue conferir na mão. Sugestão com `A = 12`, `B = 4` (em decimal):

| SelOp | Operação | Esperado (A=12, B=4) | Confira |
|-------|----------|----------------------|---------|
| 100   | A AND B  | 12 AND 4 = 4 (`00000100`) | bit a bit |
| 011   | A OR B   | 12 OR 4 = 12 (`00001100`) | bit a bit |
| 110   | A XOR B  | 12 XOR 4 = 8 (`00001000`) | bit a bit |
| 000   | A NAND B | `11111011` (251)     | bit a bit |
| 111   | A + B    | 16 (`00010000`), C=0 | soma |
| 001   | A − B    | 8 (`00001000`), C=0  | subtração |
| 010   | A × B    | 48 (`00110000`), C=0 | multiplicação |
| 101   | A ÷ B    | 3 (`00000011`), C=0  | divisão |

**Testes específicos das flags:**
- **Z:** faça `A − B` com `A = B` (ex: 5 − 5 = 0) → `S = 0` e **Z deve acender**.
- **C (overflow soma):** some dois positivos grandes em complemento de 2 (ex:
  `A = 100`, `B = 100` → 200 estoura o range −128..127) → **C deve acender**.
- **C (divisão por zero):** `B = 0` com SelOp de divisão → **C deve acender**.

Se algo der vermelho no fio: quase sempre é **largura de bits incompatível**
(esqueceu de pôr Data Bits = 8 em alguma porta) ou **dois drivers no mesmo fio**.

---

<a name="16"></a>
## 16. Checklist de entrega + o que vai no relatório

**Arquivo Logisim (.circ):** ULA funcionando, entradas com chaves, saídas com
LEDs, versão 3.9+.

**Relatório PDF — esta entrega parcial cobre boa parte:**
- [ ] Apresentação do trabalho.
- [ ] **Diagrama de blocos** da solução (use o diagrama da seção 1).
- [ ] **Tabelas-verdade** dos blocos: Full Adder (seção 6) é a principal; inclua
      também a tabela de operações do Grupo 30 (seção 1).
- [ ] **Simplificações:** as equações do Full Adder, decodificador e flags
      (seções 6, 10, 12, 13) já são a forma simplificada — mostre o raciocínio.
- [ ] **Circuitos dos blocos** com prints do Logisim (portas lógicas).
- [ ] **Equações dos blocos** (seção 17 abaixo, prontas).
- [ ] Conclusão.

> A parte da tabela "passo-a-passo das equações" (resolução das equações de 1º e
> 2º grau) e os vídeos são da **entrega final (01/07)** — não precisam agora.

---

<a name="17"></a>
## 17. Tabela-resumo de expressões (pra colar)

**Full Adder** (Análise Combinacional → inputs `a,b,cin`, outputs `s,cout`):
```
s    = a ^ b ^ cin
cout = a b + a cin + b cin
```

**Decodificador 3→8** (inputs `s2,s1,s0`, outputs `E0..E7`):
```
E0 = ~s2 ~s1 ~s0
E1 = ~s2 ~s1  s0
E2 = ~s2  s1 ~s0
E3 = ~s2  s1  s0
E4 =  s2 ~s1 ~s0
E5 =  s2 ~s1  s0
E6 =  s2  s1 ~s0
E7 =  s2  s1  s0
```

**Operações lógicas (8 bits, bit a bit):**
```
op_nand_i = ~(Ai & Bi)
op_and_i  =   Ai & Bi
op_or_i   =   Ai + Bi
op_xor_i  =   Ai ^ Bi
```

**Aritméticas:**
```
A + B   = Somador8(A, B, Cin=0)          ; overflow = Cin7 ^ Cout7
A - B   = Somador8(A, ~B, Cin=1)         ; overflow = Cin7 ^ Cout7
A × B   = Multiplier(A, B) -> 8 bits baixos ; overflow = (parte alta != 0)
A ÷ B   = Divider(A, B) -> quociente     ; overflow = (B == 0)
```

**Multiplexador de dados (saída final, por bit i):**
```
Si = E0 op_nand_i + E1 op_sub_i + E2 op_mult_i + E3 op_or_i
   + E4 op_and_i  + E5 op_div_i + E6 op_xor_i  + E7 op_soma_i
```
(Na prática você monta isso com Bit Extender + AND + OR, não digitando — seção 11.)

**Flags:**
```
Z = ~(S0 + S1 + S2 + S3 + S4 + S5 + S6 + S7)
C = E1 ov_sub + E2 ov_mult + E5 ov_div + E7 ov_soma
```

---

### Ordem prática de montagem (resumo)
1. `FullAdder` por expressão →
2. `Somador8` estrutural (expondo Cin7/Cout7) →
3. portas lógicas de 8 bits →
4. subtrator (segundo Somador8 com ~B e Cin=1) →
5. Multiplier + Divider →
6. `Decoder3x8` por expressão →
7. mux de dados (extender + AND + OR) →
8. flags Z e C →
9. pinos/chaves/LEDs no `main` →
10. testar SelOp por SelOp.

Bom trabalho. Qualquer bloco que travar na montagem (o somador e o mux costumam
ser os mais chatos), peça o detalhamento visual dele.
