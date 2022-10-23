/* We are considering Jaipur to be 20*20 matrix and marked its various co-ordinates as high traffic areas of Jaipur and user
can choose from the location list the source and destination he wants to visit and our algorithm will try to find the path to the
destination if there exists(there could be no path as some roads are blocked due to construction) and the path will would be such the
user encounters minimum traffic(not in all cases but it tends to minimize it).We are using A* Search algorithm to find the path and
have optimized so that it guides us to the nearest lowest traffic path.*/
#include<bits/stdc++.h>
using namespace std;
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
typedef pair<pair<double,double>,pair<int,int>> ppPair;
#define ROW 20
#define COL 20
int traffic[ROW][COL];//matrix to store traffic at each instant of time,like live traffic
int c=-1;
void initializeTraffic() //Initializing Traffic matrix as 0 on all roads
{
    for(int i=0;i<ROW;i++)
    {
        for(int j=0;j<COL;j++)
            traffic[i][j]=0;
    }
}
class Car{
public:
    pair<int,int>cur_pos;//stores current position of car
    pair<int,int>dest;//stores destination of each car
    stack<Pair>path;//stores the path followed by each car
    void pos(pair<int,int>&pos)//Randomly calculating source and destination of cars 
    {
        c++;
        srand(c);
        int a=rand()%20;
        int b=rand()%20;
        pos.first=a;
        pos.second=b;
        return;
    }
};
struct cell {
	// Row and Column index of its parent
	int parent_i, parent_j;
	// f = g + h
	double f, g, h;
};
 void updatePosition(Car&a)//Updating position of each car as time passes by each unit time
    {
        if(a.cur_pos.first==a.dest.first && a.cur_pos.second==a.dest.second)
		{
			return;//if car is at destination no need to move it
		}
		traffic[a.cur_pos.first][a.cur_pos.second]>0?traffic[a.cur_pos.first][a.cur_pos.second]-=1:0;//decreasing traffic as now the car is moving ahead
		if(a.path.empty())
			return;
		a.cur_pos=a.path.top();
        a.path.pop();
        traffic[a.cur_pos.first][a.cur_pos.second]+=1;//increasig traffic at new position
        return;
    }

// A Function to check whether given cell (row, col) is a valid cell or not.
bool isValid(int row, int col)
{
	// Returns true if row number and column number is in range.
	return (row >= 0) && (row < ROW) && (col >= 0)
		&& (col < COL);
}

// A Function to check whether road is blocked or not.
bool isUnBlocked(int grid[][COL], int row, int col)
{
	// Returns true if the road is not blocked else false
	if (grid[row][col] == 1)
		return (true);
	else
		return (false);
}

// A Function to check whether destination has been reached
bool isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}


// A function to return the traffic at that position
double calculateTraffic(int row, int col)
{
	return traffic[row][col];
}

// A function to store the path of each Car
void tracePath(cell cellDetails[][COL], Pair dest,Car&a)
{
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
			&& cellDetails[row][col].parent_j == col)) {
		a.path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}//Path of car is obtained
	/*Path=a.path;
	Path.push(make_pair(row,col));
	cout<<"the path is - ";
	while(!Path.empty())
	{
		Pair p=Path.top();
		Path.pop();
		cout<<p.first<<","<<p.second<<" ";
	}
	cout<<endl;*/
	return;
}
double euclidean_dist(int x1,int y1,int x2,int y2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}
//Compare function for elements to enter the priority queue according to non decreasing traffic order
struct TrafficCompare{
bool operator()(ppPair a,ppPair b)
{
	int x=traffic[a.second.first][a.second.second];
	int y=traffic[b.second.first][b.second.second];
	if(x>y)
		return true;
	else if(x<y)
		return false;
	//if traffic is same at two positions then it is sorted according to distance from destination
	return a.first.second>b.first.second;
}
};

// A* Search to find the most optimal path that a Car should follow for least traffic.
int aStarSearch(int grid[][COL], Pair src, Pair dest,Car&a,priority_queue<ppPair,vector<ppPair>,TrafficCompare>openList)
{
/*	// Either the source or the destination is blocked
	if (isUnBlocked(grid, src.first, src.second) == false
		|| isUnBlocked(grid, dest.first, dest.second)
			== false) {
		printf("Source or the destination is blocked\n");
		return;
	}*/

	// If the destination is the same as source 
	if (isDestination(src.first, src.second, dest)
		== true) {
		printf("We are already at the destination\n");
		return 1;
	}
	//closedList denotes the visited array.
	bool closedList[ROW][COL];
	memset(closedList, false, sizeof(closedList));

	cell cellDetails[ROW][COL];

	int i, j;

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implemented as a set of pair of
	pair.*/
	// Put the starting cell on the open list and set its 'f' as 0
	openList.push(make_pair(make_pair(0.0,euclidean_dist(src.first,src.second,dest.first,dest.second)), make_pair(i, j)));

	// We set this boolean value as false as initially as the destination is not reached.
	bool foundDest = false;

	while (!openList.empty()) {
		ppPair p = openList.top();

		// Remove this vertex from the open list
		openList.pop();

		// Add this vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;
		//A car can move in * direction,along the 4 directions(up,down,left,right) and along 4 diagnol directions.
		// To store the 'g', 'h' and 'f' of the 8 successors
		double gNew, hNew, fNew; 

//IF THE CAR MOVES NORTH-
		// Only process this cell if this is a valid one
		if (isValid(i - 1, j) == true) {
			// If the destination  is the same as the current successor
			if (isDestination(i - 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}
			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j] == false
					&& isUnBlocked(grid, i - 1, j)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateTraffic(i - 1, j);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
							 						OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i-1,j,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i - 1, j)));

					// Update the details of this cell
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES SOUTH-
		// Only process this cell if this is a valid one
		if (isValid(i + 1, j) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i + 1, j, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}
			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j] == false
					&& isUnBlocked(grid, i + 1, j)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateTraffic(i + 1, j);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i+1,j,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i + 1][j].f == FLT_MAX
					|| cellDetails[i + 1][j].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i + 1, j)));
					// Update the details of this cell
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES EAST-
		// Only process this cell if this is a valid one
		if (isValid(i, j + 1) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i, j + 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j + 1] == false
					&& isUnBlocked(grid, i, j + 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateTraffic(i, j + 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i,j+1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i][j + 1].f == FLT_MAX
					|| cellDetails[i][j + 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i, j+1)));

					// Update the details of this cell
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES WEST-
		// Only process this cell if this is a valid one
		if (isValid(i, j - 1) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i, j - 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i][j - 1] == false
					&& isUnBlocked(grid, i, j - 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateTraffic(i, j - 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i,j-1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i][j - 1].f == FLT_MAX
					|| cellDetails[i][j - 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i, j-1)));

					// Update the details of this cell
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES NORTH-EAST -
		// Only process this cell if this is a valid one
		if (isValid(i - 1, j + 1) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i - 1, j + 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j + 1] == false
					&& isUnBlocked(grid, i - 1, j + 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateTraffic(i - 1, j + 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i-1,j+1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i - 1][j + 1].f == FLT_MAX
					|| cellDetails[i - 1][j + 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i - 1, j+1)));
					// Update the details of this cell
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES NORTH-WEST -
		// Only process this cell if this is a valid one
		if (isValid(i - 1, j - 1) == true) {
			// If the destination cell is the same as thE current successor
			if (isDestination(i - 1, j - 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i - 1][j - 1] == false
					&& isUnBlocked(grid, i - 1, j - 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateTraffic(i - 1, j - 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i-1,j-1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i - 1][j - 1].f == FLT_MAX
					|| cellDetails[i - 1][j - 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i - 1, j-1)));
					// Update the details of this cell
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES SOUTH-EAST -
		// Only process this cell if this is a valid one
		if (isValid(i + 1, j + 1) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i + 1, j + 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j + 1] == false
					&& isUnBlocked(grid, i + 1, j + 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateTraffic(i + 1, j + 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i+1,j+1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i + 1][j + 1].f == FLT_MAX
					|| cellDetails[i + 1][j + 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i +1, j+1)));

					// Update the details of this cell
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

//IF THE CAR MOVES SOUTH-WEST -
		// Only process this cell if this is a valid one
		if (isValid(i + 1, j - 1) == true) {
			// If the destination cell is the same as the current successor
			if (isDestination(i + 1, j - 1, dest) == true) {
				// Set the Parent of the destination cell
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				tracePath(cellDetails, dest,a);
				foundDest = true;
				return 1;
			}

			// If the successor is already on the closed list or if it is blocked, then ignore it.
			// Else do the following
			else if (closedList[i + 1][j - 1] == false
					&& isUnBlocked(grid, i + 1, j - 1)
							== true) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateTraffic(i + 1, j - 1);
				fNew = gNew + hNew;

				/* If it isn’t on the open list, add it to the open list. Make the current square
				   the parent of this square. Record the f, g, and h costs of the square cell
												 OR
				   If it is on the open list already, check to see if this path to that square is
				   better, using 'f' cost as the measure.*/
				int di=euclidean_dist(i+1,j-1,dest.first,dest.second);//calculating distance from destination
				if (cellDetails[i + 1][j - 1].f == FLT_MAX
					|| cellDetails[i + 1][j - 1].f > fNew) {
					openList.push(make_pair(make_pair(
						fNew,di), make_pair(i + 1, j-1)));

					// Update the details of this cell
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	/* When the destination cell is not found and the opeN list is empty, then we conclude that we failed to
	   reach the destination cell. This may happen when there is no way to destination (due to blockage)*/
	return 0;
}
//Function to update traffic and car position of every previous car that has started its journey as each unit time passes by.
void UpdateTraffic(vector<Car>&T,int i)
{
    int j=0;
    while(j!=i)
    {
        
        updatePosition(T[j]);//call to the function that updates cur_pos and traffic matrix 
        j++;    
    }
    return;
}
//Function to print the current traffic matrix at any time
void printTraffic()
{
    for(int i=0;i<ROW;i++)
    {
        for(int j=0;j<COL;j++)
            cout<<traffic[i][j]<<" ";
        cout<<endl;
    }
}
//Function to fill grid with 0 and 1 to decide whether road is blocked or not;
int fillGrid()
{
	
	return (rand()+rand())%2;
}
//hash function to map co-ordinates to different places in Jaipur
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
//Function to print Path of a Car
void printPath(Car a,unordered_map<pair<int, int>, string, hash_pair>m)
{
	string grid[ROW][COL];
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			grid[i][j]=".";
		}
	}
	Pair a1=a.cur_pos;
	int i=1;
	grid[a1.first][a1.second]=i;
	i++;
	while(!a.path.empty())
	{
		Pair b=a.path.top();
		a.path.pop();
		grid[b.first][b.second]=i;
		i++;
	}
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			cout<<grid[i][j]<<" ";
		}
		cout<<endl;
		cout<<endl;
	}
	return;
}
int main()
{
	/* Description of the Grid-
	1--> The road is not blocked
	0--> The road is blocked */
	int grid[ROW][COL];
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			grid[i][j]=fillGrid();
		}
	}
    initializeTraffic();//Initializing initial traffic as 0 on every road

	unordered_map<pair<int, int>, string, hash_pair>m;//map to store co-ordinated of different places of Jaipur on the grid
	m[make_pair(0,0)]="Army Public school";
	m[make_pair(3,0)]="sky beach";
	m[make_pair(12,0)]="vaishali";
	m[make_pair(18,0)]="Puno";
	m[make_pair(2,1)]="country lnn";
	m[make_pair(4,1)]="JVVNL";
	m[make_pair(8,1)]="sodala";
	m[make_pair(15,1)]="elements mall";
	m[make_pair(0,2)]="Delhi Golden roadways";
	m[make_pair(3,2)]="jai mahal";
	m[make_pair(19,2)]="Family Hospital";
	m[make_pair(2,3)]="KV -4";
	m[make_pair(10,3)]="civil lines";
	m[make_pair(14,3)]="Sanjeevni Hospital";
	m[make_pair(16,3)]="Gopalpura more";
	m[make_pair(1,4)]="jaipur junction";
	m[make_pair(0,5)]="military hospital";
	m[make_pair(4,5)]="ganpati plaza";
	m[make_pair(7,5)]="c scheme";
	m[make_pair(17,5)]="Tonk fatak";
	m[make_pair(19,5)]="Mansarovar";
	m[make_pair(2,6)]="sindhi camp";
	m[make_pair(9,6)]="albert hall museum";
	m[make_pair(13,6)]="22 godam ";
	m[make_pair(4,7)]="ajmeri gate";
	m[make_pair(17,7)]="amar jawan jyoti";
	m[make_pair(3,8)]="chand pole";
	m[make_pair(18,8)]="Fun Kingdom  Amusement Park";
	m[make_pair(0,9)]="bani park";
	m[make_pair(10,9)]="Jaipur central jail";
	m[make_pair(15,9)]="central park";
	m[make_pair(19,10)]="star dust";
	m[make_pair(4,11)]="Bapu bazar";
	m[make_pair(16,11)]="moodi dungri";
	m[make_pair(2,12)]="nahargarh fort";
	m[make_pair(13,12)]="sms hospital";
	m[make_pair(18,13)]="jawar kala kendra";
	m[make_pair(9,15)]="sophia school";
	m[make_pair(14,15)]="bapu nagar";
	m[make_pair(15,15)]="aadarsh nagar ";
	m[make_pair(19,15)]="panchwati";
	m[make_pair(1,16)]="jal mahal";
	m[make_pair(5,16)]="city palace";
	m[make_pair(6,17)]="Ghat gate";
	m[make_pair(12,17)]="pink square";
	m[make_pair(16,17)]="sindhi colony";
	m[make_pair(5,18)]="badi chopati";
	m[make_pair(10,18)]="ramganj bazar";
	m[make_pair(19,18)]="jawahar nagar";
	m[make_pair(1,19)]="LNMIIT";

    vector<Car>T;//Stores each car that is on road
	priority_queue<ppPair,vector<ppPair>,TrafficCompare>openList;
    for(int i=0;i<1000;i++)//Initializing traffic of Jaipur with 500 Cars
    {
        Car a;
        a.pos(a.cur_pos);
        a.pos(a.dest);
		grid[a.cur_pos.first][a.cur_pos.second]=1;
		grid[a.dest.first][a.dest.second]=1;
        int ans=aStarSearch(grid, a.cur_pos,a.dest,a,openList);
        traffic[a.cur_pos.first][a.cur_pos.second]+=1;
        UpdateTraffic(T,i);
        T.push_back(a);
    }

	vector<string> places;//List of places user can ask to go to/from 
	places.push_back("Army Public school");
	places.push_back("sky beach");
	places.push_back("vaishali");
	places.push_back("Puno");
	places.push_back("country lnn");
	places.push_back("JVVNL");
	places.push_back("sodala");
	places.push_back("elements mall");
	places.push_back("Delhi Golden roadways");
	places.push_back("jai mahal");
	places.push_back("Family Hospital");
	places.push_back("KV -4");
	places.push_back("civil lines");
	places.push_back("Sanjeevni Hospital");
	places.push_back("Gopalpura more");
	places.push_back("jaipur junction");
	places.push_back("military hospital");
	places.push_back("ganpati plaza");
	places.push_back("c scheme");
	places.push_back("Tonk fatak");
	places.push_back("Mansarovar");
	places.push_back("sindhi camp");
	places.push_back("albert hall museum");
	places.push_back("22 godam ");
	places.push_back("ajmeri gate");
	places.push_back("amar jawan jyoti");
	places.push_back("chand pole");
	places.push_back("Fun Kingdom  Amusement Park");
	places.push_back("bani park");
	places.push_back("Jaipur central jail");
	places.push_back("central park");
	places.push_back("star dust");
	places.push_back("Bapu bazar");
	places.push_back("moodi dungri");
	places.push_back("nahargarh fort");
	places.push_back("sms hospital");
	places.push_back("jawar kala kendra");
	places.push_back("sophia school");
	places.push_back("bapu nagar");
	places.push_back("aadarsh nagar ");
	places.push_back("panchwati");
	places.push_back("jal mahal");
	places.push_back("city palace");
	places.push_back("Ghat gate");
	places.push_back("pink square");
	places.push_back("sindhi colony");
	places.push_back("badi chopati");
	places.push_back("ramganj bazar");
	places.push_back("jawahar nagar");
	places.push_back("LNMIIT");  

	int x=1;
	cout<<"Currently 500 Cars are already on road"<<endl;//We have initialized traffic with 500 cars above
	int c=500;
	while(x!=-1)
	{
		cout<<"If u wish to see the current traffic press 1"<<endl;
		cout<<"If you want to add another car press 0"<<endl;
		cout<<"Else if you want to quit press -1"<<endl;
		cin>>x;
		if(x==-1)
			break;
		else if(x==1)
			printTraffic();
		else if(x==0)
		{
			cout<<"Choose the source and destination of car from the given list of locations in Jaipur";
			for(int i=0;i<places.size();i++)
			{
				cout<<i+1<<". "<<places[i]<<endl;
			}
			int s1,s2;
			cin>>s1>>s2;
			if(s1>50 || s1<1 || s2>50 || s2<1)
			{
				cout<<"Invalid Choice"<<endl;
				continue;
			}
			string x1=places[s1-1];
			string x2=places[s2-1];
			pair<int,int> src1;
			pair<int,int> dest1;
			for(auto x= m.begin();x!=m.end();x++)
			{
				if(x->second==x1){
					src1=x->first;	
				}
				if(x->second==x2){
					dest1=x->first;
				}
			}
			int ans;
			Car a;
			a.cur_pos=src1;
			a.dest=dest1;
			grid[a.cur_pos.first][a.cur_pos.second]=1;
			grid[a.dest.first][a.dest.second]=1;
			ans=aStarSearch(grid, a.cur_pos,a.dest,a,openList);
			traffic[a.cur_pos.first][a.cur_pos.second]+=1;
        	UpdateTraffic(T,++c);
        	T.push_back(a);
			if(ans==1)
			{
				cout<<"Car "<<c<<" has reached Destination"<<endl;
				cout<<"If you want to see the path followed by Car press 1"<<endl;
				int pa;
				cin>>pa;
				if(pa==1)
				{
					printPath(a,m);
				}
			}
			else
				cout<<"Car "<<c<<" cannot reach Destination as road is blocked"<<endl;
		}
		else 
		{
			cout<<"Invalid Option"<<endl;
			continue;
		}
	}
    return 0;
}




