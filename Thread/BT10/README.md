Give one example where single thread is better than multithread: 
When multi-thread execute on single processer, it's slower than single-thread because if it's single processer, it has one thread executed at a time and when multi-thread executed, it takes more time because forking and joining tasks (context-switching). 
In reality, tasks are complicated so program multi-threading will be able to make use of multi-processor. With more single problem, single-thread sometimes is a better solution.

one example in Java:
//Single-thread
public class SingleThread {
    static long COUNTER = 0;

    public static void main(String... arrgs) {
        long start = System.currentTimeMillis();
        for(int i = 0; i < 100_000_000; i++) {
            ++COUNTER;
        }
        long end = System.currentTimeMillis();
        System.out.println("Executed in: " + (end - start));
        System.out.println(COUNTER);
    }
}

//Multi-Thread
public class MultiThread {
    static long COUNTER = 0;
    
    public static void main(String... args) throws Interrupt
    var threads = IntStream
        .range(0, 100)
        .mapToObj(ignore -> new Thread(() -> {synchronized (MultiThread.class) {
            for(int i = 0; i < 1_000_000; ++i)
                ++COUNTER;
            }
        }
    )).collect(COllectors.toList());
long start = System.currentTimeMillis();
threads.forEach(Thread::start);
for(var Thread : threads) {
    thread.join();
}
long end = System.currentTimeMillis();
System.out.println("Executed in: " + (end - start));
System.out.println(COUNTER);
}