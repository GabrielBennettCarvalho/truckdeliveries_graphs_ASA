import subprocess
import time
import matplotlib.pyplot as plt
import numpy as np
import os
import csv

# --- CONFIGURAÇÕES ---
EXECUTAVEL_PROJETO = "./projeto"  # Nome do teu executável compilado
EXECUTAVEL_GERADOR = "./gerador_p2"  # Nome do teu gerador compilado
NUM_TESTES = 30             # Quantos pontos queres no gráfico
N_INICIAL = 50                # N inicial
N_PASSO = 150                   # Quanto aumenta o N a cada teste
DENSIDADE = 40                    # Densidade das arestas (%)
TIMEOUT_LIMIT = 8.0               # Limite de tempo (segundos)

# Vetores para guardar os dados
eixo_x_complexidade = [] # N * (N + K)
eixo_y_tempos = []       # Tempo em segundos
dados_tabela = []        # Para imprimir a tabela no final

print(f"{'N':<10} | {'K (Arestas)':<12} | {'Teórica N(N+K)':<20} | {'Tempo (s)':<10}")
print("-" * 60)

for i in range(NUM_TESTES):
    n = N_INICIAL + (i * N_PASSO)
    m = max(10, n // 10) 
    seed = i + 1
    
    # 1. Gerar o Input
    comando_gerador = [EXECUTAVEL_GERADOR, str(n), str(m), str(DENSIDADE), str(seed)]
    
    try:
        # Executa o gerador
        processo_gen = subprocess.run(comando_gerador, capture_output=True, text=True, check=True)
        input_data = processo_gen.stdout
        
        # 2. Extrair o K (Arestas)
        linhas = input_data.strip().split()
        # O K é o 5º elemento (índice 4)
        k_arestas = int(linhas[4])
        
        # Calcular Complexidade Teórica: N * (N + K)
        val_teorico = n * (n + k_arestas)
        
        # 3. Executar o Projeto e Medir Tempo
        start_time = time.perf_counter()
        processo_proj = subprocess.run(
            [EXECUTAVEL_PROJETO], 
            input=input_data, 
            capture_output=True, 
            text=True,
            timeout=TIMEOUT_LIMIT
        )
        end_time = time.perf_counter()
        
        tempo_execucao = end_time - start_time
        
        # Guardar dados
        eixo_x_complexidade.append(val_teorico)
        eixo_y_tempos.append(tempo_execucao)
        
        dados_tabela.append([n, k_arestas, val_teorico, tempo_execucao])
        
        print(f"{n:<10} | {k_arestas:<12} | {val_teorico:<20.2e} | {tempo_execucao:.4f}")

    except subprocess.TimeoutExpired:
        print(f"Teste com N={n} demorou demasiado. A parar.")
        break
    except Exception as e:
        print(f"Erro no teste N={n}: {e}")
        break

# --- GERAR GRÁFICO ---
plt.figure(figsize=(10, 6))

# 1. DADOS REAIS (Azul com linha e pontos)
# 'o-' significa: marcador circular (o) ligado por linha sólida (-)
plt.plot(eixo_x_complexidade, eixo_y_tempos, 'o-', color='blue', label='Dados Reais')

# 2. TENDÊNCIA (Vermelha tracejada)
if len(eixo_x_complexidade) > 1:
    z = np.polyfit(eixo_x_complexidade, eixo_y_tempos, 1)
    p = np.poly1d(z)
    
    # Aqui alterei a label como pediste
    plt.plot(eixo_x_complexidade, p(eixo_x_complexidade), "r--", label='Teórica $N(N+K)$')

plt.title("Tempo de Execução vs Complexidade Teórica")
plt.xlabel("Complexidade Teórica: $N \\times (N + K)$")
plt.ylabel("Tempo (segundos)")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

nome_csv = "tabela_resultados.csv"
with open(nome_csv, mode='w', newline='') as file:
    writer = csv.writer(file)
    # Cabeçalho
    writer.writerow(["N", "K (Arestas)", "Teorica N(N+K)", "Tempo (s)"])
    # Dados
    writer.writerows(dados_tabela)

print(f"\n[SUCESSO] Tabela exportada para: {nome_csv}")

# Guardar o gráfico
output_filename = "grafico_complexidade.png"
plt.savefig(output_filename)
print(f"\nGráfico guardado como: {output_filename}")
# plt.show() # Podes desconentar se quiseres que abra a janela