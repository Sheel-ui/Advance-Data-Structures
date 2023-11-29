import java.util.ArrayList;

public class MinHeap {

    private ArrayList<Ride> list;

    // default constructor
    public MinHeap() {
        this.list = new ArrayList<Ride>();
    }

    // size of minheap
    public int size(){
        return list.size();
    }

    // insert ride object
    public void insert(Ride r) {

        // set min Heap index and add at the end

        r.setIndex(list.size());
        list.add(r);
        int i = list.size() - 1;
        int parent = parent(i);

        // heapifying
        while (parent != i && list.get(i).rideCost <= list.get(parent).rideCost) {
            boolean flag = true;
            if (list.get(i).rideCost == list.get(parent).rideCost) {
                // ride cost anf ride duratiion tie breaking condition
                if (list.get(i).tripDuration > list.get(parent).tripDuration) {
                    flag = false;
                    break;
                }
            }
            if (flag == true) {
                // swap
                swap(i, parent);
                i = parent;
                parent = parent(i);
            }
        }
    }


    // method called by Update Trip when trip duration is reduced we need to check the tie breaking condition again
    public void decreaseDuration(int rideIndex, int tripDuration) {
        list.get(rideIndex).setTripDuration(tripDuration);
        int parent = parent(rideIndex);
        
        while (rideIndex != 0 && list.get(rideIndex).rideCost <= list.get(parent).rideCost) {
            boolean flag = true;
            if (list.get(rideIndex).rideCost == list.get(parent).rideCost) {
                if (list.get(rideIndex).tripDuration > list.get(parent).tripDuration) {
                    flag = false;
                    break;
                }
            }
            if (flag == true) {
                swap(rideIndex, parent);
                rideIndex = parent;
                parent = parent(rideIndex);
            }
        }
    }

    // remove top element, add last lement at the top, apply heapify at 0th index
    public Ride removeMin() {

        if (list.size() == 0) {

            throw new IllegalStateException("MinHeap is EMPTY");
        } else if (list.size() == 1) {

            Ride min = list.remove(0);
            return min;
        }

        Ride min = list.get(0);
        Ride lastItem = list.remove(list.size() - 1);

        // updating heap index
        lastItem.setIndex(0);
        list.set(0, lastItem);
        

        // heapify at 0th index
        heapify(0);

        return min;
    }

    // arbitrary remove
    public void remove(int heapIndex) {
        int minCost = list.get(0).rideCost;
        list.get(heapIndex).setRideCost(minCost - 1);

        // set the ride cost of deleting node at minimum cost and heapify it
        int parent = parent(heapIndex);
        while (list.get(heapIndex).rideCost < list.get(parent).rideCost) {
            swap(heapIndex, parent);
            heapIndex = parent;
            parent = parent(parent);
        }

        // node to  delete is at the top so delete
        removeMin();
    }


    // code for heapifying
    private void heapify(int i) {

        int left = left(i);
        int right = right(i);
        int smallest = -1;

        // find smallest key at left
        if (left <= list.size() - 1 && list.get(left).rideCost < list.get(i).rideCost) smallest = left;
        else smallest = i;

        // find smallest key at right
        if (right <= list.size() - 1 && list.get(right).rideCost < list.get(smallest).rideCost) {
            smallest = right;
        }

        // current key != smallest key move downward
        if (smallest != i) {

            swap(i, smallest);
            heapify(smallest);
        }
    }


    // index of left right and parent
    private int right(int i)  {return 2*i+2;}

    private int left(int i) {return 2 * i + 1;}

    private int parent(int i) {
        if (i % 2 == 1) return i / 2;
        return (i - 1) / 2;
    }


    // swapping the objects and exchanging the index
    private void swap(int i, int parent) {

        Ride a = list.get(parent);
        a.setIndex(i);

        Ride b = list.get(i);
        b.setIndex(parent);
        
        list.set(parent, b);
        list.set(i, a);
    }

}