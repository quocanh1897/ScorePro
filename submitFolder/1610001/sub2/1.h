#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

void BubbleSort(int *a, int n) {
	int flag = true;
	for (int i = 0; i<n - 1; i++) {
		flag = true;
		for (int j = n - 1; j>i; j--)
			if (a[j] < a[j - 1]) {
				int t = a[j];
				a[j] = a[j - 1];
				a[j - 1] = t;
				flag = false;
			}
		if (flag)
			break;
	}
}



#endif /* BUBBLESORT_H_ */
#pragma once
