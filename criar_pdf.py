from fpdf import FPDF

# --- DADOS DO RELATÓRIO ---
TITULO = "Relatório 2º Projeto ASA 2025/2026"
GRUPO = "TP010"
ALUNOS = "Gabriel Carvalho (113981) e Gustavo Ribeiro (114139)"

# --- TEXTOS E FÓRMULAS ---
DESC_TEXTO_1 = (
    "A solução proposta modela o mapa como um Grafo Acíclico Dirigido (DAG). "
    "O objetivo é calcular o número de caminhos entre cada par (u, v) para determinar "
    "o ID do camião. A abordagem utiliza Programação Dinâmica combinada com "
    "Ordenação Topológica. A recorrência principal propaga o número de caminhos de "
    "um nó 'u' para os seus vizinhos 'v':"
)

FORMULA_1 = "paths[v] = (paths[v] + paths[u]) % M"

DESC_TEXTO_2 = (
    "O algoritmo itera sobre cada nó 'i' como origem. Para cada origem, reinicia-se o vetor "
    "de caminhos e percorre-se o grafo na ordem topológica. Se, para uma aresta (i, u), "
    "o número de caminhos resultar num ID de camião válido (dentro do intervalo [m1, m2]), "
    "a rota é registada. Segundo o enunciado do problema, o ID é calculado por:"
)

FORMULA_2 = "ID_camiao = 1 + (paths[u] % M)"

# Análise Teórica
TEORICA_PONTOS = [
    "Leitura e Construção do Grafo: O(N + K). Apenas uma passagem pela entrada para criar listas de adjacência.",
    "Ordenação Topológica: O(N + K). Cada nó e aresta são processados exatamente uma vez.",
    "Cálculo dos Caminhos (Ciclo Principal): O(N * (N + K)). O algoritmo executa N iterações (uma para cada nó origem). No pior caso, percorre-se todo o subgrafo alcançável.",
    "Escrita dos Resultados: O(L log L). Onde L é o total de entregas válidas encontradas. É necessário ordenar a lista de pares (origem, destino) para cada camião antes da impressão.",
    "Complexidade Global: O(N * (N + K))."
]

EXPERIMENTAL_TEXTO = (
    "Foram realizados testes com instâncias aleatórias variando N de 50 a 4400, de 150 em 150,"
    "mantendo a densidade de arestas constante (40%). O gráfico abaixo compara o "
    "tempo de execução real com a complexidade teórica prevista."
)

# A FRASE NOVA QUE PEDISTE
CONCLUSAO_FINAL = (
    "É possível observar uma relação linear entre a complexidade teórica prevista e os "
    "tempos registados, confirmando que a implementação está de acordo com a análise teórica."
)

# Dados da Tabela
TABELA_DADOS = [
    ["N", "K (Arestas)", "Teorica N(N+K)", "Tempo (s)"],
    ["50", "488", "2.69e+04", "0.002"],
    ["500", "49,918", "2.52e+07", "0.011"],
    ["1100", "241,723", "2.67e+08", "0.103"],
    ["1550", "480,327", "7.47e+08", "0.240"],
    ["2000", "799,044", "1.60e+09", "0.415"],
    ["2450", "1,199,682", "2.95e+09", "0.703"],
    ["3050", "1,860,124", "5.68e+09", "1.315"],
    ["3500", "2,449,108", "8.58e+09", "2.097"],
    ["4400", "3,868,958", "1.70e+10", "4.227"],
]

class PDF(FPDF):
    def header(self):
        self.set_font('Arial', 'B', 10)
        self.cell(0, 10, 'Instituto Superior Técnico - Análise e Síntese de Algoritmos', 0, 1, 'C')
        self.ln(5)

    def footer(self):
        self.set_y(-15)
        self.set_font('Arial', 'I', 8)
        self.cell(0, 10, f'Página {self.page_no()}', 0, 0, 'C')

    def chapter_title(self, label):
        self.set_font('Arial', 'B', 12)
        self.set_fill_color(240, 240, 240)
        self.cell(0, 8, label, 0, 1, 'L', 1)
        self.ln(4)

    def chapter_body(self, text):
        self.set_font('Arial', '', 10)
        self.multi_cell(0, 5, text)
        self.ln(2)
        
    def formula_block(self, formula):
        self.set_font('Courier', 'B', 11)
        self.set_x(20)
        self.cell(0, 8, formula, 0, 1, 'L')
        self.ln(2)

    def txt(self, text):
        return text.encode('latin-1', 'replace').decode('latin-1')

# --- CONSTRUÇÃO DO PDF ---
pdf = PDF()
pdf.set_margins(25, 25, 25)
pdf.add_page()

# Cabeçalho Principal
pdf.set_font('Arial', 'B', 16)
pdf.cell(0, 10, pdf.txt(TITULO), 0, 1, 'C')
pdf.set_font('Arial', '', 11)
pdf.cell(0, 6, pdf.txt(f"Grupo: {GRUPO}"), 0, 1, 'C')
pdf.cell(0, 6, pdf.txt(f"Alunos: {ALUNOS}"), 0, 1, 'C')
pdf.ln(8)

# 1. Descrição
pdf.chapter_title(pdf.txt("Descrição do Problema e da Solução"))
pdf.chapter_body(pdf.txt(DESC_TEXTO_1))
pdf.formula_block(FORMULA_1)
pdf.chapter_body(pdf.txt(DESC_TEXTO_2))
pdf.formula_block(FORMULA_2)
pdf.ln(1)

# 2. Análise Teórica
pdf.chapter_title(pdf.txt("Análise Teórica"))
pdf.set_font('Arial', '', 10)
for ponto in TEORICA_PONTOS:
    pdf.cell(5, 5, "-", 0, 0) 
    pdf.multi_cell(0, 5, pdf.txt(ponto))
    pdf.ln(1)
pdf.ln(3)

# 3. Avaliação Experimental
pdf.chapter_title(pdf.txt("Avaliação Experimental dos Resultados"))
pdf.chapter_body(pdf.txt(EXPERIMENTAL_TEXTO))

# Inserir Gráfico
try:
    # Ajusta para caber bem na página
    pdf.image('grafico_complexidade.png', x=35, w=140)
    pdf.ln(5)
except:
    pdf.cell(0, 10, "[ERRO: Imagem grafico_complexidade.png nao encontrada]", 0, 1, 'C')

# Desenhar Tabela
pdf.set_font('Arial', 'B', 9)
w = [20, 35, 45, 30] # Larguras das colunas
left_margin = (210 - 50 - sum(w)) / 2 + 25 # Centralizar
pdf.set_x(left_margin)

# Cabeçalho Tabela
for i, head in enumerate(TABELA_DADOS[0]):
    pdf.cell(w[i], 7, pdf.txt(head), 1, 0, 'C')
pdf.ln()

# Linhas Tabela
pdf.set_font('Arial', '', 9)
for row in TABELA_DADOS[1:]:
    pdf.set_x(left_margin)
    for i, col in enumerate(row):
        pdf.cell(w[i], 6, col, 1, 0, 'C')
    pdf.ln()

# --- ADICIONAR A CONCLUSÃO DEPOIS DA TABELA ---
pdf.ln(5)
pdf.set_font('Arial', '', 10)
pdf.multi_cell(0, 5, pdf.txt(CONCLUSAO_FINAL))

# Guardar
output_file = "Relatorio_Projeto2_Final.pdf"
pdf.output(output_file)
print(f"PDF criado com sucesso: {output_file}")