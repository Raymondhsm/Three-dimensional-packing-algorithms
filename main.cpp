#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


struct Cargo {
	int length;
	int width;
	int height;
	int weigh;
	int direction;
	int x;
	int y;
	int z;
	char type;
}a, b, c;

struct Box {
	int length;
	int width;
	int height;
	int weigh;
}box1, box2;

struct Space {
	int fx;
	int fy;
	int fz;
	int bx;
	int by;
	int bz;

	int bulk() {
		return (bx - fx)*(by - fy)*(bz - fz);
	}

	Space *front;
	Space *next;
	Space *head;
	vector<Space*> mSR;
};

bool put(Cargo cargo, Space *SR, vector<Cargo> path);
Space* copy(Space *link);
bool isHas(Space *mSR, Space nmSR);
void printPath(vector<Cargo> path);
Cargo changeDirection(Cargo cargo, int direction);
bool isPlane(Space sp);

int main() {
	a.length = 14;
	a.width = 7;
	a.height = 5;
	a.type = 'A';

	b.length = 5;
	b.width = 8;
	b.height = 5;
	b.type = 'B';

	c.length = 12;
	c.width = 7;
	c.height = 4;
	c.type = 'C';

	Cargo cargoArray[3] = { a,b,c };

	box1.length = 8;
	box1.width = 10;
	box1.height = 14;

	box2.length = 24;
	box2.width = 20;
	box2.height = 20;

	Box box = box2;

	Space *head = new Space();
	head->fx = 0;
	head->fy = 0;
	head->fz = 0;
	head->bx = box.length;
	head->by = box.width;
	head->bz = box.height;
	head->head = head;
	head->next = NULL;
	head->front = NULL;


	Cargo cargo;
	vector<Cargo> path;

	for (int i = 0; i < 3 ; i++) {
		//for (int j = 0; j < 6; j++) {
			put(changeDirection(cargoArray[i],0), copy(head), path);
			put(changeDirection(cargoArray[i], 2), copy(head), path);
		//}
	}

	system("pause");
}

void sortSR(Space *sp) {
	if (sp == NULL)return;

	Space *b = sp;
	Space *a = sp->next;
	while (b->next != NULL) {
		a = b->next;

		while (true) {
			if (b->bulk() > a->bulk()) {
				/*if (a->front != NULL)a->front->next = a->next;
				else sp = a->next;
				a->next->front = a->front;
				a->front = a->next;
				a->next = a->next->next;
				a->next->next->front = a;
				a->next->next = a;*/

				/*Space *ftob = new Space(*b);
				Space *btof = new Space(*a);

				if (b->front != NULL)b->front->next = a;
				else sp = a;
				if (b->next != NULL)b->next->front = a;

           				if (a->front != NULL)a->front->next = b;
				else sp = b;
				if (a->next != NULL)a->next->front = b;

				b->front = btof->front;
				b->next = btof->next;

				a->front = ftob->front;
				a->next = ftob->next;*/

				Space *ftob = new Space(*b);

				b->fx = a->fx; b->fy = a->fy; b->fz = a->fz;
				b->bx = a->bx; b->by = a->by; b->bz = a->bz;
				b->mSR = a->mSR;

				a->fx = ftob->fx; a->fy = ftob->fy; a->fz = ftob->fz;
				a->bx = ftob->bx; a->by = ftob->by; a->bz = ftob->bz;
				a->mSR = ftob->mSR;

				delete ftob;
			}
			
			if (a->next == NULL)break;
			a = a->next;
		}
		b = b->next;
	}
}

Space* split(Space *space, Cargo cargo,int &counter) {
	Space *a = new Space[4];
	Space b;
	counter = 0;

	b.fx = space->fx; b.fy = space->fy; b.fz = space->fz + cargo.height;
	b.bx = space->fx + cargo.length; b.by = space->fy + cargo.width; b.bz = space->bz;
	a[0] = b;

	b.fx = space->fx; b.fy = space->fy + cargo.width; b.fz = space->fz;
	b.bx = space->fx + cargo.length; b.by = space->by; b.bz = space->bz;
	a[1] = b;

	b.fx = space->fx + cargo.length; b.fy = space->fy; b.fz = space->fz;
	b.bx = space->bx; b.by = space->fy + cargo.width; b.bz = space->bz;
	a[2] = b;

	b.fx = space->fx + cargo.length; b.fy = space->fy + cargo.width; b.fz = space->fz;
	b.bx = space->bx; b.by = space->by; b.bz = space->bz;
	a[3] = b;

	a[0].front = NULL;
	a[0].next = &a[1];
	a[1].front = &a[0];
	a[1].next = &a[2];
	a[2].front = &a[1];
	a[2].next = &a[3];
	a[3].front = &a[2];
	a[3].next = NULL;

	for (int i = 0; i < 4; i++) {
		if (a[i].fx == a[i].bx || a[i].fy == a[i].by || a[i].fz == a[i].bz) {
			if (i < 3)a[i + 1].front = a[i].front;
			if (i > 0)a[i - 1].next = a[i].next;
			else a = &a[i + 1];
			
		}
		else {
			counter++;
		}
	}

	return a;
}

void SRtomSR(Space *SR, Space *mSR) {
	Space *head = SR;
	Space *mhead = mSR;
	while (head->next != NULL) {
		if (isPlane(*head)) {
			head = head->next;
			continue;
		}

		//比较SR
		Space *space = head->next;
		do {
			if ((head->bx == space->fx || head->fx == space->bx)&&head->fz == space->fz&&head->by > space->fy&&space->by > head->fy) {
				Space newmSR;
				newmSR.fx = min(head->fx, space->fx);
				newmSR.fy = max(head->fy, space->fy);
				newmSR.fz = head->fz;
				newmSR.bx = max(head->bx, space->bx);
				newmSR.by = min(head->by, space->by);
				newmSR.bz = min(head->bz, space->bz);

				if (mhead->head == NULL) {
					newmSR.head = &newmSR;
					newmSR.next = NULL;
					newmSR.front = NULL;

					*mhead = newmSR;
				}
				else {
					newmSR.head = mSR;
					newmSR.next = NULL;
					newmSR.front = mhead;

					mhead->next = new Space(newmSR);
					mhead = mhead->next;
				}

				head->mSR.push_back(mhead);
				space->mSR.push_back(mhead);
				mhead->mSR.push_back(head);
				mhead->mSR.push_back(space);
			}
			else if ((head->by == space->fy || head->fy == space->by)&&head->fz == space->fz&&head->bx > space->fx&&space->bx > head->fx) {
				Space newmSR;
				newmSR.fx = max(head->fx, space->fx);
				newmSR.fy = min(head->fy, space->fy);
				newmSR.fz = head->fz;
				newmSR.bx = min(head->bx, space->bx);
				newmSR.by = max(head->by, space->by);
				newmSR.bz = min(head->bz, space->bz);

				if (mhead->head == NULL) {
					newmSR.head = &newmSR;
					newmSR.next = NULL;
					newmSR.front = NULL;

					*mhead = newmSR;
				}
				else {
					newmSR.head = mSR;
					newmSR.next = NULL;
					newmSR.front = mhead;

					mhead->next = new Space(newmSR);
					mhead = mhead->next;
				}

				head->mSR.push_back(mhead);
				space->mSR.push_back(mhead);
				mhead->mSR.push_back(head);
				mhead->mSR.push_back(space);
			}

			//结束条件
			if (space->next == NULL)break;
			space = space->next;
		} while (true);


		//比较SR和mSR
		space = mSR;
		do {
			if (space == NULL)break;
			if ((head->bx == space->fx || head->fx == space->bx) && head->fz == space->fz&&head->by > space->fy&&space->by > head->fy) {
				Space newmSR;
				newmSR.fx = min(head->fx, space->fx);
				newmSR.fy = max(head->fy, space->fy);
				newmSR.fz = head->fz;
				newmSR.bx = max(head->bx, space->bx);
				newmSR.by = min(head->by, space->by);
				newmSR.bz = min(head->bz, space->bz);

				if (mhead->head == NULL) {
					newmSR.head = &newmSR;
					newmSR.next = NULL;
					newmSR.front = NULL;

					if (isHas(mSR, newmSR))break;
					*mhead = newmSR;

				}
				else {
					newmSR.head = mSR;
					newmSR.next = NULL;
					newmSR.front = mhead;

					mhead->next = new Space(newmSR);
					mhead = mhead->next;
				}

				head->mSR.push_back(mhead);
				mhead->mSR.push_back(head);
			}
			else if ((head->by == space->fy || head->fy == space->by) && head->fz == space->fz&&head->bx > space->fx&&space->bx > head->fx) {
				Space newmSR;
				newmSR.fx = max(head->fx, space->fx);
				newmSR.fy = min(head->fy, space->fy);
				newmSR.fz = head->fz;
				newmSR.bx = min(head->bx, space->bx);
				newmSR.by = max(head->by, space->by);
				newmSR.bz = min(head->bz, space->bz);

				if (mhead->head == NULL) {
					newmSR.head = &newmSR;
					newmSR.next = NULL;
					newmSR.front = NULL;

					if (isHas(mSR,newmSR))break;
					*mhead = newmSR;

				}
				else {
					newmSR.head = mSR;
					newmSR.next = NULL;
					newmSR.front = mhead;

					if (isHas(mSR, newmSR))break;
					mhead->next = new Space(newmSR);
					mhead = mhead->next;
				}

				head->mSR.push_back(mhead);
				mhead->mSR.push_back(head);
			}

			//结束条件
			if (space->next == NULL)break;
			space = space->next;
		} while (true);

		head = head->next;
	}
}

bool isPlane(Space sp) {
	if (sp.fx == sp.bx || sp.fy == sp.by || sp.fz == sp.bz)
		return true;
	else return false;
}

bool isHas(Space *mSR,Space nmSR) {
	if (mSR == NULL)return false;
	Space *sp = mSR;

	do {
		if (sp->fx == nmSR.fx&&sp->fy == nmSR.fy&&sp->fz == nmSR.fz&&sp->bx == nmSR.bx&&sp->by == nmSR.by&&sp->bz == nmSR.bz)
			return true;

		if (sp->next == NULL)break;
		sp = sp->next;
	} while (true);

	return false;
}

void printPath(vector<Cargo> path)
{
	cout<< "方案：\t";
	for (int i = 0; i < path.size(); i++) {
		cout << path[i].type << " (" << path[i].x << "," << path[i].y << "," << path[i].z << "," << path[i].direction << ") -> ";
	}
	cout << "\r\n";
}

Cargo changeDirection(Cargo cargo, int direction)
{
	Cargo cg = cargo;

	switch (direction) {
	case 0:
		break;

	case 1:
		cargo.width = cg.height;
		cargo.height = cg.width;
		cargo.direction = 1;
		break;

	case 2:
		cargo.length = cg.width;
		cargo.width = cg.length;
		cargo.direction = 2;
		break;

	case 3:
		cargo.length = cg.width;
		cargo.width = cg.height;
		cargo.height = cg.length;
		cargo.direction = 3;
		break;

	case 4:
		cargo.length = cg.height;
		cargo.width = cg.length;
		cargo.height = cg.width;
		cargo.direction = 4;
		break;

	case 5:
		cargo.length = cg.height;
		cargo.height = cg.length;
		cargo.direction = 5;
		break;
	}

	return cargo;
}

Space* copy(Space *link) {
	Space *head = new Space(*link);
	Space *tolink = link;
	Space *a = head;
	Space *front = NULL;
	while (tolink->next != NULL) {
		a->next = new Space(*tolink->next);
		a->front = front;
		front = a;
		a = a->next;
		tolink = tolink->next;
	}
	a->front = front;
	return head;
}

bool put(Cargo cargo, Space *SR, vector<Cargo> path) {
	Space *mSR = new Space();
	SRtomSR(SR, mSR);
	bool isPut = false;

	sortSR(SR);
	Space *space = SR;
	do {
		if (SR == NULL)break;
		if (cargo.length <= space->bx - space->fx && cargo.width <= space->by - space->fy && cargo.height <= space->bz - space->fz) {
			isPut = true;
			cargo.x = space->fx;
			cargo.y = space->fy;
			cargo.z = space->fz;


			//分割
			int counter;
			Space *a = split(space, cargo, counter);

			//删除有效空间记录
			if (space->front != NULL)space->front->next = space->next;
			else SR = space->next;
			if (space->next != NULL)space->next->front = space->front;


			//更新SR
			Space *tail = new Space(*a);
			while (tail->next != NULL)tail = tail->next;

 			tail->next = SR;
			if (SR != NULL)SR->front = tail;
			SR = a;

			//cout << "SR装载（" << cargo.length << "," << cargo.width << "," << cargo.height << ")成功" << endl;

			break;
		}
		else {
			//cout << "SR装载（" << cargo.length << "," << cargo.width << "," << cargo.height << ")失败" << endl;
			//printPath(path);
		}

		if (space->next == NULL)break;
		space = space->next;
	} while (true);

	sortSR(mSR);
	space = mSR;
	do {
		if (mSR == NULL || isPut)break;
		if (cargo.length <= space->bx - space->fx && cargo.width <= space->by - space->fy && cargo.height <= space->bz - space->fz) {
			isPut = true;
			cargo.x = space->fx;
			cargo.y = space->fy;
			cargo.z = space->fz;

			//分割
			int counter;
			Space *a = split(space, cargo, counter);

			//删除有效空间记录
			for (int i = 0; i < space->mSR.size(); i++) {
				if (space->mSR[i]->front != NULL)space->mSR[i]->front->next = space->mSR[i]->next;
				else SR = space->mSR[i]->next;
				if (space->mSR[i]->next != NULL)space->mSR[i]->next->front = space->mSR[i]->front;
			}
			/*if (space->front != NULL)space->front->next = space->next;
			else SR = space->next;*/

			//更新SR
			Space *tail = new Space(*a);
			while (tail->next != NULL)tail = tail->next;

			tail->next = SR;
			if (SR != NULL)SR->front = tail;
			SR = a;

			//cout << "mSR装载（" << cargo.length << "," << cargo.width << "," << cargo.height << ")成功" << endl;

			break;
		}
		else {
			//cout << "mSR装载（" << cargo.length << "," << cargo.width << "," << cargo.height << ")失败" << endl;
			//printPath(path);
		}

		if (space->next == NULL)break;
		space = space->next;
	} while (true);


	if (isPut) path.push_back(cargo);
	else {
		//delete SR;
		//delete mSR;
		return false;
	}


	bool isEnd = true;
	//for (int j = 0; j < 6; j++) {
		isEnd = put(changeDirection(a, 0), copy(SR), path) && isEnd;
		isEnd = put(changeDirection(a, 2), copy(SR), path) && isEnd;
	//}

	//for (int j = 0; j < 6; j++) {
		isEnd = put(changeDirection(b, 0), copy(SR), path) && isEnd;
		isEnd = put(changeDirection(b, 2), copy(SR), path) && isEnd;
	//}

	//for (int j = 0; j < 6; j++) {
		isEnd = put(changeDirection(c, 0), copy(SR), path) && isEnd;
		isEnd = put(changeDirection(c, 2), copy(SR), path) && isEnd;
	//}

	if (!isEnd && path.size() > 0)printPath(path);
	return true;
}

