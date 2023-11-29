import java.io.File;
import java.io.FileNotFoundException;  
import java.util.Scanner; 
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.Exception;
import java.util.List;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;

class gatorTaxi {


    // Insert tuple to minheap and RBT
    public String insert(int rideNumber,int rideCost,int tripDuration,MinHeap minHeap,RbTree rbt){
        // Check duplicate
        if(rbt.searchTree(rideNumber).rideNumber==0){
            Ride ride = new Ride(rideNumber,rideCost,tripDuration);
            minHeap.insert(ride);
            rbt.insert(ride);
            return "";
        }
        else{
            return "Duplicate RideNumber";
        }
    }

    // search ride number in RBT and return value
    public String print(int rideNumber, RbTree rbt){
        Ride ride = rbt.searchTree(rideNumber);
        return "("+ride.rideNumber+","+ride.rideCost+","+ride.tripDuration+")";
    }

    // Print level order traversal from Ride Number 1 to Ride Number 2
    public String print(int rideNumber1, int rideNumber2, RbTree rbt){
        return rbt.getRides(rideNumber1,rideNumber2);
    }

    // Remove top from min Heap and remove the same from RBT
    public String getNextRide(MinHeap minHeap,RbTree rbt){
        if(minHeap.size()==0) return "No active ride requests";
        else {
            Ride r = minHeap.removeMin();
            rbt.deleteRide(r.rideNumber);
            return "("+r.rideNumber+","+r.rideCost+","+r.tripDuration+")";
        }
    }

    // three conditions mention in assignment for Update Tip
    public void updateTrip(int rideNumber, int tripDuration, MinHeap minHeap, RbTree rbt) {
        Ride r = rbt.searchTree(rideNumber);
        if(r.rideNumber!=0){
            int cost = r.rideCost;
            if (r.heapIndex!=-1) {
                int current = r.tripDuration;
                int rideIndex = r.heapIndex;
                int updated = tripDuration;
                // condition 1, new value < old value, update duration and adjust heap
                if (updated<=current){
                    minHeap.decreaseDuration(rideIndex,updated);
                }
                // condition 2, old value < new value <= 2 * old value, remove node and add new node with cost+10
                else if (current < updated && updated <= 2* current) {
                    rbt.deleteRide(r.rideNumber);
                    minHeap.remove(r.heapIndex);
                    insert(r.rideNumber,cost+10,updated,minHeap,rbt);
                }
                // condition 3: new value > 2*old value, delete node
                else if (2*current < updated) {
                    rbt.deleteRide(r.rideNumber);
                    minHeap.remove(r.heapIndex);
                    
                }
            }
        }
    }

    // cancel ride is simply delering the node from min Heap and RBT
    public void cancelRide(int rideNumber, MinHeap minHeap, RbTree rbt) {
        Ride r = rbt.searchTree(rideNumber);
        if(r.heapIndex!=-1){
            rbt.deleteRide(r.rideNumber);
            minHeap.remove(r.heapIndex);
        }
    }

    public static void main(String arg[]){
        ArrayList<String> list =new ArrayList<String>();
        try {

            // file reader and writer

            File myObj = new File(arg[0]);
            Scanner myReader = new Scanner(myObj);
            FileWriter fileWriter = new FileWriter("output_file.txt");
            PrintWriter printWriter = new PrintWriter(fileWriter);
            
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                list.add(data);
            }

            // INIT gatorTaxi, minHeap and RBT

            gatorTaxi r = new gatorTaxi();
            MinHeap m = new MinHeap();
            RbTree rbt = new RbTree();
            String s = "";

            for(int i = 0; i < list.size(); i++) {

                String inp = list.get(i);
                String[] arr = inp.split("\\(",2);
                String operation = arr[0];
                String arguments = arr[1].substring(0,arr[1].length()-1);
                String[] args = arguments.split(",",10);
                boolean flag = false;
                // Switch cases for operations
                switch(operation) {
                    case "Insert":
                        s = r.insert(Integer.valueOf(args[0]),Integer.valueOf(args[1]),Integer.valueOf(args[2]),m,rbt);
                        if(s.length()>0){
                            printWriter.print(s);
                            printWriter.print("\n");
                            if(s.equalsIgnoreCase("Duplicate RideNumber")){
                                // Break when Duplicate Ride Found
                                flag=true;
                            }
                        }
                        break;
                    case "Print":
                        if(args.length==1){
                            s = r.print(Integer.valueOf(args[0]),rbt);
                            printWriter.print(s);
                            printWriter.print("\n");
                        }
                        else{
                            s = r.print(Integer.valueOf(args[0]),Integer.valueOf(args[1]),rbt);
                            printWriter.print(s);
                            printWriter.print("\n");
                        }
                        break;
                    case "GetNextRide":
                        s = r.getNextRide(m,rbt);
                        printWriter.print(s);
                        printWriter.print("\n");
                        break;
                    case "CancelRide":
                        r.cancelRide(Integer.valueOf(args[0]),m,rbt);
                        break;
                    case "UpdateTrip":
                        r.updateTrip(Integer.valueOf(args[0]),Integer.valueOf(args[1]),m,rbt);
                        break;
                }
                if(flag==true){
                    break;
                }
            }
            
            printWriter.close();
            myReader.close();
        } catch (Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}