import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Lavanderia {

    private static final int NUM_MAQUINAS_LAVAR = 3;
    private static final int NUM_SECADORAS = 3;
    private static final int NUM_TABUAS = 2;
    private static final int BUFFER_SIZE = 5;

    private static final Semaphore semLavarFull = new Semaphore(0);
    private static final Semaphore semLavarEmpty = new Semaphore(BUFFER_SIZE);
    private static final Semaphore semLavarMutex = new Semaphore(1);

    private static final Semaphore semSecarFull = new Semaphore(0);
    private static final Semaphore semSecarEmpty = new Semaphore(BUFFER_SIZE);
    private static final Semaphore semSecarMutex = new Semaphore(1);

    private static final Semaphore semPassarFull = new Semaphore(0);
    private static final Semaphore semPassarEmpty = new Semaphore(BUFFER_SIZE);
    private static final Semaphore semPassarMutex = new Semaphore(1);

    private static final Queue<Integer> buf_lavar = new LinkedList<>();
    private static final Queue<Integer> buf_secar = new LinkedList<>();
    private static final Queue<Integer> buf_passar = new LinkedList<>();

    private static long startTime;

    private static volatile boolean running = true;

    public static void main(String[] args) throws InterruptedException {
        startTime = System.currentTimeMillis();

        if (args.length < 1) {
            System.out.println("Forneça o caminho do arquivo com a lista de nomes.");
            return;
        }

        String filePath = args[0];

        System.out.println("Início da simulação");

        Thread clienteThread = new Thread(() -> processarClientes(filePath));
        clienteThread.start();

        Thread[] maquinasThreads = new Thread[NUM_MAQUINAS_LAVAR];
        for (int i = 0; i < NUM_MAQUINAS_LAVAR; i++) {
            final int id = i;
            maquinasThreads[i] = new Thread(() -> maquinaLavar(id));
            maquinasThreads[i].start();
        }

        Thread[] secadorasThreads = new Thread[NUM_SECADORAS];
        for (int i = 0; i < NUM_SECADORAS; i++) {
            final int id = i;
            secadorasThreads[i] = new Thread(() -> secadora(id));
            secadorasThreads[i].start();
        }

        Thread[] tabuasThreads = new Thread[NUM_TABUAS];
        for (int i = 0; i < NUM_TABUAS; i++) {
            final int id = i;
            tabuasThreads[i] = new Thread(() -> tabuaPassar(id));
            tabuasThreads[i].start();
        }

        clienteThread.join();

        running = false;

        for (Thread t : maquinasThreads) {
            t.join();
        }
        for (Thread t : secadorasThreads) {
            t.join();
        }
        for (Thread t : tabuasThreads) {
            t.join();
        }

        System.out.println("Fim da simulação");
    }

    private static void processarClientes(String filePath) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String linha;
            int mudaId = 0;

            while ((linha = reader.readLine()) != null) {
                System.out.printf("%s colocou sua muda: %d (Início: %dms)%n",
                        linha, mudaId, tempoMilisegundos());
                semLavarEmpty.acquire();
                semLavarMutex.acquire();
                buf_lavar.add(mudaId++);
                semLavarMutex.release();
                semLavarFull.release();
            }
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void maquinaLavar(int id) {
        try {
            while (running || !buf_lavar.isEmpty()) {
                semLavarFull.acquire();
                semLavarMutex.acquire();
                Integer muda = buf_lavar.poll();
                semLavarMutex.release();
                semLavarEmpty.release();

                if (muda != null) {
                    System.out.println("MUDA " + muda + " sendo lavada na MÁQUINA DE LAVAR " + id);
                    Thread.sleep(400);
                    System.out.println("MÁQUINA DE LAVAR " + id + " LIVRE");

                    semSecarEmpty.acquire();
                    semSecarMutex.acquire();
                    buf_secar.add(muda);
                    semSecarMutex.release();
                    semSecarFull.release();
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void secadora(int id) {
        try {
            while (running || !buf_secar.isEmpty()) {
                semSecarFull.acquire();
                semSecarMutex.acquire();
                Integer muda = buf_secar.poll();
                semSecarMutex.release();
                semSecarEmpty.release();

                if (muda != null) {
                    System.out.println("MUDA " + muda + " está secando no SECADOR " + id);
                    Thread.sleep(100);
                    System.out.println("SECADOR " + id + " LIVRE");

                    semPassarEmpty.acquire();
                    semPassarMutex.acquire();
                    buf_passar.add(muda);
                    semPassarMutex.release();
                    semPassarFull.release();
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void tabuaPassar(int id) {
        try {
            while (running || !buf_passar.isEmpty()) {
                semPassarFull.acquire();
                semPassarMutex.acquire();
                Integer muda = buf_passar.poll();
                semPassarMutex.release();
                semPassarEmpty.release();

                if (muda != null) {
                    System.out.println("MUDA " + muda + " está sendo passada na TÁBUA " + id);
                    Thread.sleep(200);
                    System.out.println("TÁBUA DE PASSAR " + id + " LIVRE");
                    System.out.println("MUDA " + muda + " terminou! (Fim: " + tempoMilisegundos() + "ms)");
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static int tempoMilisegundos() {
        return (int) ((System.currentTimeMillis() - startTime));
    }
}
