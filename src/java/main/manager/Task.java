package manager;

import java.util.concurrent.Callable;
import java.util.function.Supplier;

public class Task implements Callable<Frame>, Supplier<Frame> {
    float[] arr;
    int index;

    public Task(float[] arr, int index) {
        this.arr = arr;
        this.index = index;
    }

    @Override
    public Frame call() throws Exception {
        return new Frame(arr, index);
    }

    @Override
    public Frame get() {
        return null;
    }
}
