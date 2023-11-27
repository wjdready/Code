public class Main {

    static {
        System.loadLibrary("add");
    }

    public native int add(int a, int b);

    public static void main(String[] args) {
        Main m = new Main();
        int result = m.add(1, 2);
        System.out.println("1 + 2 = " + result);
    }
}
