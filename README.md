# Amazon Engineering Challenge: Árvores Balanceadas (AVL e Rubro-Negra)

Protótipo de alta performance desenvolvido em **C++17** para o gerenciamento logístico de identificadores de encomendas durante picos de tráfego (Black Friday), implementando do zero e comparando empiricamente as estruturas **Árvore AVL** e **Árvore Rubro-Negra (*Red-Black Tree*)**.

---

## Identificação Acadêmica

| Campo | Informação |
| :--- | :--- |
| **Instituição** | Universidade Federal do Rio Grande do Norte (UFRN) — Instituto Metrópole Digital (IMD) |
| **Disciplina** | IMD0039 — Estruturas de Dados Básicas II |
| **Docente** | Profa. Ligia Francielle Borges |
| **Autor (Discente)** | Leonardo Xavier Cruz Filho |
| **Matrícula** | `20250049025` |
| **Repositório Público** | [https://github.com/leonardoxaviercf/edb-ii-atividade-2](https://github.com/leonardoxaviercf/edb-ii-atividade-2) |

---

## 1. Contexto do Problema

Durante a **Black Friday**, o sistema de logística da Amazon precisa registrar, localizar e remover identificadores de pacotes de forma massiva e contínua. Caso os identificadores cheguem em lotes sequenciais (ordem crescente ou decrescente), uma Árvore de Busca Binária (BST) comum sofre degeneração estrutural, transformando-se em uma lista encadeada com complexidade de pior caso $O(n)$ por operação.

Para garantir latência previsível em tempo logarítmico $O(\log n)$ independentemente da ordem de chegada dos dados, este projeto implementa e avalia duas estratégias clássicas de balanceamento dinâmico sem o uso de bibliotecas prontas:
1. **Árvore AVL:** balanceamento estrito baseado na altura das subárvores.
2. **Árvore Rubro-Negra (`RBTree`):** balanceamento heurístico baseado em propriedades de coloração de nós.

---

## 2. Funcionalidades Implementadas

O sistema atende integralmente aos requisitos funcionais e de análise especificados:

* **Seleção Dinâmica em Tempo de Execução:** menu interativo que permite escolher entre a **Árvore AVL** e a **Árvore Rubro-Negra**, além de modos automatizados de validação e benchmark.
* **Inserção de Elementos (`insert`):** registro de chaves inteiras com rebalanceamento automático (rotações simples/duplas na AVL e recolorações/rotações na Rubro-Negra).
* **Remoção de Elementos (`remove`):** tratamento completo dos 3 casos estruturais (remoção de nó folha com $0$ filhos, nó com $1$ filho e nó interno com $2$ filhos utilizando o sucessor em-ordem), preservando todas as invariantes de balanceamento.
* **Busca de Elementos (`search`):** localização rápida em $O(\log n)$ com contabilização do número de comparações realizadas.
* **Percurso em Ordem (`inOrder`):** listagem de todos os identificadores registrados em sequência estritamente crescente.
* **Impressão Visual da Estrutura (`printTree`):** renderização hierárquica no terminal com ramificações ASCII:
  * Na **Árvore AVL**, exibe a chave, a altura atual e o fator de balanceamento: `(h:X, fb:Y)`.
  * Na **Árvore Rubro-Negra**, identifica claramente a cor de cada nó via códigos ANSI e marcadores explícitos: `[V]` para **Vermelho** (*Red*) e `[P]` para **Preto** (*Black*).
* **Autovalidação Matemática (`validateProperties`):** auditoria recursiva que verifica se as propriedades de BST, o fator de balanceamento $|FB| \le 1$ (AVL), a ausência de nós vermelhos consecutivos e a uniformidade da altura negra (Rubro-Negra) permanecem 100% íntegros após qualquer operação.
* **Telemetria Nativa (`TreeMetrics`):** instrumentação embutida que contabiliza rotações simples à esquerda, rotações simples à direita, rotações duplas (`LR` e `RL`), ajustes de cor (*color flips*) e comparações de chaves.

---

## 3. Decisões de Implementação e Funcionamento

Todas as decisões arquiteturais foram tomadas visando segurança de memória, determinismo nos testes e fidelidade à literatura clássica (Cormen et al. — CLRS):

1. **Tratamento de Elementos Repetidos (Duplicatas):**
   * No domínio logístico da Amazon, cada identificador de encomenda (`ID`) é único. Tentativas de inserir uma chave já existente na árvore são detectadas durante a descida de busca e **rejeitadas de forma segura** (`insert` retorna `false`), mantendo a árvore e seus fatores de balanceamento inalterados e notificando o usuário no terminal.
2. **Busca e Remoção de Elementos Inexistentes e Árvores Vazias:**
   * Operações de busca ou remoção sobre árvores vazias ou para chaves não registradas retornam `false` imediatamente, sem disparar exceções ou falhas de segmentação (*segmentation faults*), exibindo mensagens informativas claras.
3. **Estratégia da Árvore AVL (`AVLTree`):**
   * Cada nó armazena sua altura $h$. O Fator de Balanceamento é definido como $FB(n) = h(\text{esq}) - h(\text{dir})$.
   * Sempre que $|FB| = 2$ após uma inserção ou remoção, o equilíbrio é restaurado por uma das quatro operações: Rotação Simples à Direita (Caso Esquerda-Esquerda), Rotação Simples à Esquerda (Caso Direita-Direita), Rotação Dupla Esquerda-Direita (`LR`) ou Rotação Dupla Direita-Esquerda (`RL`).
4. **Estratégia da Árvore Rubro-Negra e Nó Sentinela (`RBTree`):**
   * Foi adotado um **nó sentinela único `NIL`** (sempre com cor `BLACK`) para representar todas as folhas externas e o pai da raiz, em substituição a ponteiros `nullptr`.
   * Essa decisão elimina checagens condicionais redundantes de ponteiro nulo durante os algoritmos `insertFixup` e `deleteFixup`, garantindo robustez total na avaliação de tios, irmãos e sobrinhos durante remoções complexas.

---

## 4. Estrutura do Repositório

O repositório inclui tanto o código-fonte completo quanto a versão pré-compilada do binário Linux em `bin/amazon_trees`:

```text
.
├── bin/
│   └── amazon_trees          # Versão compilada e funcional do programa (executável Linux x86_64)
├── include/
│   ├── BalancedTree.hpp      # Interface polimórfica comum, cores ANSI e struct TreeMetrics
│   ├── AVLTree.hpp           # Declaração da classe AVLTree e estrutura de nós
│   ├── RBTree.hpp            # Declaração da classe RBTree com nó sentinela NIL
│   └── TestSuite.hpp         # Declaração da bateria de testes de borda e benchmark comparativo
├── src/
│   ├── AVLTree.cpp           # Implementação das operações e rotações da Árvore AVL
│   ├── RBTree.cpp            # Implementação das operações, fixups e cores da Rubro-Negra
│   ├── TestSuite.cpp         # Casos de teste automatizados e benchmark (N = 1000)
│   └── main.cpp              # Menu principal interativo e ponto de entrada do sistema
├── Makefile                  # Script de automação de compilação e execução
└── README.md                 # Documentação técnica do projeto
```

---

## 5. Dependências e Instruções de Execução

### Pré-requisitos e Dependências
* **Sistema Operacional:** Linux, macOS ou Windows (via WSL / MinGW).
* **Compilador:** `g++` (GCC) com suporte ao padrão **C++17** ou superior (não há dependências de bibliotecas externas; utiliza apenas a biblioteca padrão `STL` para entrada/saída e geração de vetores de teste).
* **Utilitário de Build (Opcional):** `GNU Make`.

### Opção A: Executar a Versão Já Compilada (Diretamente do Repositório)
Em ambientes Linux (`x86_64`), o binário já se encontra compilado no diretório `bin/`. Basta garantir permissão de execução e rodá-lo:

```bash
chmod +x ./bin/amazon_trees
./bin/amazon_trees
```

### Opção B: Compilar e Executar via `Makefile`
Na raiz do projeto, utilize os comandos abaixo:

```bash
# Compila todo o projeto e gera o binário em ./bin/amazon_trees
make

# Compila (se necessário) e inicia o programa imediatamente
make run

# Remove arquivos objetos temporários e recompila do zero
make clean && make
```

### Opção C: Compilação Manual (Sem `Make`)
Caso deseje compilar manualmente em uma única linha usando o `g++`:

```bash
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -pedantic -O2 -Iinclude src/*.cpp -o bin/amazon_trees
./bin/amazon_trees
```

---

## 6. Exemplos de Uso

Ao iniciar o executável `./bin/amazon_trees`, o menu principal oferece 5 modos de operação:

```text
============================================================
  AMAZON ENGINEERING CHALLENGE - ARVORES BALANCEADAS
============================================================
  1. Utilizar Arvore AVL (Modo Interativo)
  2. Utilizar Arvore Rubro-Negra (Modo Interativo)
  3. Executar Bateria de Testes de Borda e Validacao (Secao 3)
  4. Comparacao Visual Lado a Lado (N = 10 Crescente)
  5. Executar Benchmark Comparativo Black Friday (N = 1000)
  0. Encerrar programa
```

### Exemplo 1: Visualização da Árvore AVL vs. Rubro-Negra (Lote `10 20 30 40 50 25`)
Inserindo a mesma sequência nas duas estruturas (Opções `1` e `2` do menu):

* **Saída na Árvore AVL** (aplica rotação dupla `RL` ao inserir o `25`, resultando em altura $3$ e $3$ rotações):
```text
└── 30 (h:3, fb:0)
    ├── 20 (h:2, fb:0)
    │   ├── 10 (h:1, fb:0)
    │   └── 25 (h:1, fb:0)
    └── 40 (h:2, fb:-1)
        └── 50 (h:1, fb:0)
-> Percurso em ordem crescente: 10 20 25 30 40 50
-> Propriedades AVL preservadas? SIM
```

* **Saída na Árvore Rubro-Negra** (resolve a inserção do `25` apenas recolorindo nós em $O(1)$, sem rotacionar, resultando em $2$ rotações totais e $12$ ajustes de cor):
```text
└── 20 [P]
    ├── 10 [P]
    └── 40 [V]
        ├── 30 [P]
        │   ├── 25 [V]
        └── 50 [P]
-> Percurso em ordem crescente: 10 20 25 30 40 50
-> Propriedades Rubro-Negras preservadas? SIM
```

### Exemplo 2: Bateria Automatizada de Casos de Borda (Opção `3`)
Executa e valida sequencialmente nas duas árvores:
1. Busca, remoção e impressão em **árvore vazia**.
2. Inserção, busca, tentativa de duplicata e remoção em **árvore com apenas $1$ elemento**.
3. Busca de elementos existentes e inexistentes.
4. Remoção de nó folha ($0$ filhos — ID `10`), nó com $1$ filho (ID `20`), nó com $2$ filhos (ID `45`) e remoção de chave inexistente (ID `888`), confirmando `validateProperties() == true` após cada etapa.

---

## 7. Resumo da Análise Comparativa e Recomendação Técnica

A **Opção `5`** do sistema submete ambas as árvores exatamente aos mesmos conjuntos de dados ($N = 1000$ encomendas em ordem **crescente**, **decrescente** e **aleatória** com semente fixa `std::mt19937(42)`), seguidos de $1200$ buscas e remoção de $30\%$ dos pacotes:

| Métrica Avaliada ($N = 1000$) | Árvore AVL | Árvore Rubro-Negra | Comportamento Observado |
| :--- | :---: | :---: | :--- |
| **Altura Pós-Inserção (Ordenada)** | $10$ | $18$ | A AVL mantém altura mínima estrita ($\le 1{,}44 \log_2 n$). |
| **Altura Pós-Inserção (Aleatória)** | $12$ | $13$ | Profundidades muito próximas em cargas não ordenadas. |
| **Rotações na Inserção (Aleatória)** | $469$ | $298$ | A Rubro-Negra realizou **$36\%$ menos rotações**. |
| **Total de Rotações (Inserção + Remoção)** | $684$ | $412$ | A Rubro-Negra exigiu **$40\%$ menos rotações** totais. |
| **Ajustes de Cor (*Color Flips*)** | $0$ | $1340$ | Trocas de bits $O(1)$ substituem rotações estruturais caras. |
| **Validação de Propriedades** | `SIM (100%)` | `SIM (100%)` | Invariantes preservadas em todos os cenários. |

### Conclusão para a Engenharia da Amazon
* **Recomendação para o Fluxo de Logística *Inbound/Outbound* (Escrita/Remoção Intensiva): Árvore Rubro-Negra.** Durante o pico da Black Friday, o registro e a baixa contínua de pacotes exigem custo mínimo de mutação estrutural. A Rubro-Negra garante limite constante de no máximo $2$ rotações por inserção e $3$ por remoção, reduzindo em $40\%$ as alterações de ponteiros em comparação à AVL.
* **Recomendação para Catálogos de Consulta (*Read-Heavy*): Árvore AVL.** Ideal para tabelas de rotas, CEPs e catálogos de produtos que sofrem poucas alterações e milhões de consultas, onde a menor altura da AVL reduz o número de comparações por busca.

---

## 8. Referências Bibliográficas

1. CORMEN, T. H.; LEISERSON, C. E.; RIVEST, R. L.; STEIN, C. **Algoritmos: Teoria e Prática**. 3. ed. Rio de Janeiro: Elsevier, 2012. (Capítulos 12 e 13 — Árvores de Busca Binária e Árvores Rubro-Negras).
2. SZWARCFITER, J. L.; MARKENZON, L. **Estruturas de Dados e seus Algoritmos**. 3. ed. Rio de Janeiro: LTC, 2010.