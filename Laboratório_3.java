import java.util.Scanner;

public class Pi extends Thread{
    public int id_threads;
    public double valor;
    public int n;
    public int n_threads;

    //Criando o construtor
    public Pi(int n, int id_threads){
        this.id_threads = id_threads;
        this.n = n;
    }

    //Método executado pela thread
    public void run(){
        for (int i = id_threads; i < n; i += n_threads){
            valor += Math.pow((-1), i) * (1.0 / (2 * i + 1));
        }
    }

    public double getValor(){
        return this.valor;
    }

    //Classe do método Main
    public static class valorPi{
        static double soma_total = 0;
        static double pi_original = Math.PI;

        public static void main (String[] args){
            Scanner sc = new Scanner(System.in);

            System.out.print("Valor de n: ");
            int n = sc.nextInt();
            System.out.print("Numero de Threads: ");
            int n_threads = sc.nextInt();

            //Criando as Threads
            Pi[] threads = new Pi[n_threads];

            for(int i = 0; i < threads.length; i++){
                threads[i] = new Pi(n, i);
                threads[i].n_threads = n_threads;
            }

            //Iniciando as Threads
            for(int i = 0; i < threads.length; i++){
                threads[i].start();
            }

            //Esperando as Threads terminarem
            for (int i = 0; i < threads.length; i++){
                try{
                    threads[i].join();
                }
                catch (InterruptedException e){
                    return;
                }
            }

            for(int i = 0; i < threads.length; i++){
                Pi t = threads[i];
                soma_total += t.getValor();
            }

            soma_total = soma_total * 4;

            //Comparando os valores
            System.out.println("Pi = " + Math.PI);
            System.out.println("Valor Obtido = " + soma_total);
            System.out.println("Erro Relativo = "+ Math.abs(pi_original-soma_total) / pi_original);
            System.out.println("Terminou..");

            sc.close();
        }
    }
}
