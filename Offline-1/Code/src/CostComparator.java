import java.util.Comparator;

public class CostComparator implements Comparator<Boards> {
    @Override
    public int compare(Boards o1, Boards o2) {
        return Integer.compare(o1.getDistance()+o1.getMoves(), o2.getDistance()+o2.getMoves());
    }
}
