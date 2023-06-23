import math
from threading import Thread
import time

class Pi(Thread):
    def __init__(self, n, id_threads, n_threads):
        Thread.__init__(self)
        self.id_threads = id_threads
        self.n = n
        self.n_threads = n_threads
        self.valor = 0

    def run(self):
        for i in range(self.id_threads, self.n, self.n_threads):
            self.valor += math.pow((-1), i) * (1.0 / (2 * i + 1))

    def getValor(self):
        return self.valor

if __name__ == "__main__":
    n = int(input("Valor de n: "))
    n_threads = int(input("Número de Threads: "))

    #Criando as threads
    threads = []
    for i in range(n_threads):
        threads.append(Pi(n, i, n_threads))

    #Iniciando as threads
    start_time = time.time()
    for thread in threads:
        thread.start()

    #Esperando as threads terminarem
    for thread in threads:
        thread.join()

    #Somando os valores das threads
    soma_total = 0
    for thread in threads:
        soma_total += thread.getValor()

    soma_total = soma_total * 4

    #Comparando os valores
    print("Pi =", math.pi)
    print("Valor Obtido =", soma_total)
    print("Erro Relativo =", abs(math.pi - soma_total) / math.pi)
    
    #Tempo de execução
    end_time = time.time()
    execution_time = end_time - start_time
    print("Tempo de Execução:", execution_time, "segundos")
