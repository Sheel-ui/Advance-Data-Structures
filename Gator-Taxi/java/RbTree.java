class RbTree {

	// root node and Null node
	private Ride NULLNODE;
    private Ride root;

	// simple binary tree searching
	private Ride searchHelper(Ride ride, int key) {
		if (ride == NULLNODE || key == ride.rideNumber) return ride;
		// if key is less, go left
		if (key < ride.rideNumber) return searchHelper(ride.left, key);
		// else go right
		return searchHelper(ride.right, key);
	}

	// balance the tree abter Delete
	private void deleteFix(Ride x) {
		Ride s;
		while (x != root && x.color == 0) {
			if (x == x.parent.left) {
				s = x.parent.right;
				if (s.color == 1) {
					// case 3.1
					s.color = 0;
					x.parent.color = 1;
					rotateLeft(x.parent);
					s = x.parent.right;
				}

				if (s.left.color == 0 && s.right.color == 0) {
					// case 3.2
					s.color = 1;
					x = x.parent;
				} else {
					if (s.right.color == 0) {
						// case 3.3
						s.left.color = 0;
						s.color = 1;
						rotateRight(s);
						s = x.parent.right;
					} 

					// case 3.4
					s.color = x.parent.color;
					x.parent.color = 0;
					s.right.color = 0;
					rotateLeft(x.parent);
					x = root;
				}
			} else {
				s = x.parent.left;
				if (s.color == 1) {
					// case 3.1
					s.color = 0;
					x.parent.color = 1;
					rotateRight(x.parent);
					s = x.parent.left;
				}

				if (s.right.color == 0 && s.right.color == 0) {
					// case 3.2
					s.color = 1;
					x = x.parent;
				} else {
					if (s.left.color == 0) {
						// case 3.3
						s.right.color = 0;
						s.color = 1;
						rotateLeft(s);
						s = x.parent.left;
					} 

					// case 3.4
					s.color = x.parent.color;
					x.parent.color = 0;
					s.left.color = 0;
					rotateRight(x.parent);
					x = root;
				}
			} 
		}
		x.color = 0;
	}

	// Transform deletion code
	private void Transform(Ride u, Ride v){
		// parent is null then set v as root
		// else check if u and left of parent
		if (u.parent == null) root = v;
		else if (u == u.parent.left) u.parent.left = v;
		else u.parent.right = v;
		v.parent = u.parent;
	}

	// delete funtion code
	private void deleteRideHelper(Ride ride, int key) {
		Ride z = NULLNODE;
		Ride x, y;
		while (ride != NULLNODE){
			// normal BST delete
			if (ride.rideNumber == key) z = ride;
			if (ride.rideNumber <= key) ride = ride.right;
			else ride = ride.left;
		}

		if (z == NULLNODE) return;

		// check conditions for rb Transplant- left and right
		y = z;
		int yOriginalColor = y.color;
		if (z.left == NULLNODE) {
			x = z.right;
			Transform(z, z.right);
		} else if (z.right == NULLNODE) {
			x = z.left;
			Transform(z, z.left);
		} else {
			y = minimum(z.right);
			yOriginalColor = y.color;
			x = y.right;
			if (y.parent == z) {
				x.parent = y;
			} else {
				Transform(y, y.right);
				y.right = z.right;
				y.right.parent = y;
			}

			Transform(z, y);
			y.left = z.left;
			y.left.parent = y;
			y.color = z.color;
		}
		// fix delete if original color is red
		if (yOriginalColor == 0) deleteFix(x);
	}
	
	// balance the tree after insert
	private void insertFix(Ride k){
		Ride u;
		while (k.parent.color == 1) {
			if (k.parent == k.parent.parent.right) {
				u = k.parent.parent.left; // uncle
				if (u.color == 1) {
					// case 3.1
					u.color = 0;
					k.parent.color = 0;
					k.parent.parent.color = 1;
					k = k.parent.parent;
				} else {
					// left of parent - right rotate
					if (k == k.parent.left) {
						k = k.parent;
						rotateRight(k);
					}
					k.parent.color = 0;
					k.parent.parent.color = 1;
					rotateLeft(k.parent.parent);
				}
			} else {
				// parent sibling
				u = k.parent.parent.right;

				if (u.color == 1) {
					u.color = 0;
					k.parent.color = 0;
					k.parent.parent.color = 1;
					k = k.parent.parent;	
				} else {
					// right of parent - left rotate
					if (k == k.parent.right) {
						k = k.parent;
						rotateLeft(k);
					}
					k.parent.color = 0;
					k.parent.parent.color = 1;
					rotateRight(k.parent.parent);
				}
			}
			if (k == root) break;
		}
		root.color = 0;
	}

	// default constructor
	public RbTree() {
		NULLNODE = new Ride();
		NULLNODE.color = 0;
		NULLNODE.left = null;
		NULLNODE.right = null;
		root = NULLNODE;
	}

	// inorder traversal from ride number 1 to ride number 2
    public String getRides(int rideNumber1, int rideNumber2) {
        String s = printBetweenRides(rideNumber1,rideNumber2,this.root);
        if(s.length()>0) return s.substring(0,s.length()-1);
        else return "(0,0,0)";
	} 

	// inorder traversal
    public String printBetweenRides(int rideNumber1, int rideNumber2, Ride ride){
            String leftS; String rightS;
			// go left till null
            if (ride.left != NULLNODE) leftS = printBetweenRides(rideNumber1,rideNumber2,ride.left);
            else leftS = "";
			// go right till null
            if (ride.right != NULLNODE) rightS = printBetweenRides(rideNumber1,rideNumber2,ride.right);
            else rightS = "";
			// if value between ride1 and ride 2 add to solution
            if (rideNumber1 <=ride.rideNumber && rideNumber2 >=ride.rideNumber) return leftS + "(" +String.valueOf(ride.rideNumber) +","+ String.valueOf(ride.rideCost) +","+ String.valueOf(ride.tripDuration) +"),"+ rightS;
            else return leftS + rightS;
    }

	// search in tree
	public Ride searchTree(int k) {
		return searchHelper(this.root, k);
	}

	// finding minimum, same as BST
	public Ride minimum(Ride ride) {
		while (ride.left != NULLNODE) ride = ride.left;
		return ride;
	}

	// finding maximum, same as BST
	public Ride maximum(Ride ride) {
		while (ride.right != NULLNODE) ride = ride.right;
		return ride;
	}

	// left right rotation of Ride x
	public void rotateLeft(Ride x) {
		Ride y = x.right;
		x.right = y.left;
		if (y.left != NULLNODE) y.left.parent = x;
		y.parent = x.parent;
		if (x.parent == null) this.root = y;
		else if (x == x.parent.left) x.parent.left = y;
		else x.parent.right = y;
		y.left = x;
		x.parent = y;
	}

	// right rotation of Ride x
	public void rotateRight(Ride x) {
		Ride y = x.left;
		x.left = y.right;
		if (y.right != NULLNODE) y.right.parent = x;
		y.parent = x.parent;
		if (x.parent == null) this.root = y;
		else if (x == x.parent.right) x.parent.right = y;
		else x.parent.left = y;
		y.right = x;
		x.parent = y;
	}

	// insert same as BST
	// balance the tree after insert
	public void insert(Ride ride) {

		ride.parent = null;
		ride.left = NULLNODE;
		ride.right = NULLNODE;
		ride.color = 1;

		Ride y = null;
		Ride x = this.root;

		while (x != NULLNODE) {
			y = x;
			if (ride.rideNumber < x.rideNumber) x = x.left;
			else x = x.right;
		}

		// parent is y
		ride.parent = y;
		if (y == null) root = ride;
		else if (ride.rideNumber < y.rideNumber) y.left = ride;
		else y.right = ride;

		// return if parent null
		if (ride.parent == null){
			ride.color = 0;
			return;
		}

		// return if parent of parent is null
		if (ride.parent.parent == null) return;

		// Balance
		insertFix(ride);
	}

	// get the root
	public Ride getRoot(){
		return this.root;
	}

	// delete function
	public void deleteRide(int data) {
		deleteRideHelper(this.root, data);
	}

}