/*
	WARNING: It does not handle negative numbers and will segfault into oblivion.
	So, hashmap is necessary, but who said that I can't create my own?
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

#define twosumdataSize	(3)
const int twosumtarget = 0;
const int twosumdata[twosumdataSize] = {0,3,-3};

typedef struct sumpair {
	int a;
	int b;
} sumpair;

sumpair findTwosumDumbWay(const int target, const int* data, const size_t dataSize) {

	sumpair result = {0, 0};

	for (size_t m = 0; m < dataSize; m++) {
		for (size_t n = 0; n < dataSize; n++) {
			if (data[m] + data[n] == target) {
				result.a = m;
				result.b = n;
				break;
			}
		}
	}
	
	return result;
}

sumpair findTwosumSmortWay(const int target, const int* data, const size_t dataSize) {

	int maxInt = 0;
	int minInt = 0;

	for (size_t i = 0; i < dataSize; i++) {
		if (data[i] > maxInt) maxInt = data[i];
		if (data[i] < minInt) minInt = data[i];
	}

	int indexShift = abs(minInt);
	size_t delta = maxInt + indexShift;

	const size_t poolSize = (delta + 1) * sizeof(int);
	int* diffs = malloc(poolSize);
	memset(diffs, 0, poolSize);

	for (size_t i = 0; i < dataSize; i++) {
		diffs[data[i] + indexShift] = target - data[i];
	}

	sumpair result = {-1, -1};

	for (size_t m = 0; m < dataSize; m++) {

		const int val = data[m];
		const int idx = val + indexShift;

		if (val + diffs[idx] == target) {

			result.a = m;
			const int diff = diffs[idx];

			for (size_t n = 0; n < dataSize; n++) {

				if (data[n] != diff) continue;
				if (m == n) continue;

				result.b = n;
				free(diffs);
				return result;
			}
		}
	}

	free(diffs);
	memset(&result, -1, sizeof(result));
	return result;
}

int main(int argc, char const *argv[]) {

	printf("Target number: %i\n", twosumtarget);


	/**
	 * Brutforce
	*/
	printf("Brutforce method:\n");

	time_t startTime = timeGetTime();

	sumpair anwser = findTwosumDumbWay(twosumtarget, twosumdata, twosumdataSize);

	//	repeat op more times to get more precise timings
	for (size_t i = 0; i < twosumdataSize; i++) {
		anwser = findTwosumDumbWay(twosumtarget, twosumdata, twosumdataSize);
	}

	printf("Anwser: %i + %i\n", anwser.a, anwser.b);
	printf("Time: %1.3f ms\n", ((timeGetTime() - startTime) / (float)twosumdataSize));


	/**
	 * Val map
	*/
	printf("\nValue map method:\n");

	startTime = timeGetTime();
	anwser = findTwosumSmortWay(twosumtarget, twosumdata, twosumdataSize);

	//	repeat op more times to get more precise timings
	for (size_t i = 0; i < twosumdataSize; i++) {
		anwser = findTwosumSmortWay(twosumtarget, twosumdata, twosumdataSize);
	}

	printf("Anwser: %i + %i\n", anwser.a, anwser.b);
	printf("Time: %1.3f ms\n", ((timeGetTime() - startTime) / (float)twosumdataSize));

	return 0;
}

/*
int* twoSum(int* nums, int numsSize, int target, int* returnSize){

	*returnSize = 0;

	size_t maxInt = 0;

	for (size_t i = 0; i < numsSize; i++) {
		if (nums[i] > maxInt) maxInt = nums[i];
	}

	const size_t poolSize = (maxInt + 1) * sizeof(poolitem);
	poolitem* pool = malloc(poolSize);
	memset(pool, 0, poolSize);

	for (size_t i = 0; i < numsSize; i++) {
		if (nums[i] > target) {
			pool[nums[i]].valid = false;
			continue;
		}
		pool[nums[i]].valid = true;
		pool[nums[i]].diff = target - nums[i];
	}

	int* result = malloc(2 * sizeof(int));

	for (size_t m = 0; m < numsSize; m++) {

		const int val = nums[m];

		if (!pool[val].valid) continue;

		if (val + pool[val].diff == target) {

			result[0] = m;
			const int diff = pool[val].diff;

			for (size_t n = 0; n < numsSize; n++) {

				if (nums[n] == diff) {
					
					if (m == n) continue;

					result[1] = n;
					free(pool);
					*returnSize = 2;
					return result;
				}
			}
		}
	}

	free(pool);
	return result;
}*/


/*
Bit test data sample
18,50,11,97,49,42,59,2,79,20,8,7,69,36,83,93,95,9,66,32,42,80,83,43,82,65,91,41,44,82,34,20,72,34,1,30,23,86,66,22,47,28,90,79,58,30,87,85,22,8,26,10,73,76,9,8,65,49,73,10,71,77,13,70,68,68,62,57,38,81,68,77,98,23,90,16,21,7,96,91,96,68,3,22,43,8,46,59,3,89,100,60,49,22,82,75,78,35,61,27,82,41,73,96,100,11,39,43,70,88,57,41,4,2,71,29,98,30,68,39,13,11,84,55,75,35,53,50,90,39,98,41,18,69,82,19,28,38,15,94,69,96,25,49,62,2,85,69,25,93,100,100,84,87,57,65,8,52,27,48,63,56,61,51,98,31,11,96,2,60,63,89,70,16,74,98,2,60,65,39,49,22,60,52,98,68,45,27,32,96,56,68,5,89,96,44,82,82,27,45,86,52,53,20,28,50,55,74,12,91,50,46,5,23,64,29,82,22,48,24,30,51,50,4,35,100,75,99,42,59,23,87,48,95,54,83,83,1,9,80,53,34,81,44,11,99,85,33,37,58,38,60,91,56,95,84,96,49,91,29,35,97,57,45,34,49,51,37,24,66,56,94,89,41,67,24,82,36,46,9,38,60,90,100,30,22,26,24,10,38,18,72,18,95,23,13,75,11,27,21,1,60,37,60,60,41,23,79,79,83,94,25,79,3,31,18,10,72,62,49,66,72,15,13,3,24,98,73,55,59,61,51,11,51,66,97,32,20,17,55,32,50,100,61,87,93,1,88,17,9,64,51,2,86,46,42,87,89,62,95,62,8,90,31,98,84,69,75,4,26,50,90,69,15,29,93,79,11,55,21,100,30,31,1,36,1,82,89,37,2,93,99,86,92,80,20,14,41,84,81,78,35,62,71,86,81,76,24,79,28,41,37,44,12,38,42,52,80,71,50,40,70,83,76,86,76,1,51,76,51,1,60,52,9,100,83,35,62,90,30,50,89,26,67,4,23,24,31,27,97,18,92,43,6,96,41,33,80,77,43,30,77,7,34,43,63,25,60,28,61,71,2,39,88,1,46,26,86,46,100,75,81,57,85,52,80,67,36,29,79,81,90,55,19,29,46,95,79,98,52,68,46,29,54,24,44,95,5,99,15,61,75,20,37,86,78,52,13,78,20,95,20,15,56,50,44,85,7,77,47,76,80,25,3,4,81,25,9,29,23,6,12,22,100,55,23,39,48,62,51,53,8,41,57,11,41,71,83,78,25,57,72,5,64,97,49,50,65,69,30,34,56,12,83,52,44,70,20,7,57,20,4,50,12,8,22,83,4,39,2,53,31,51,29,24,88,99,69,58,28,76,80,92,49,76,54,16,87,37,80,84,67,54,7,46,87,37,53,2,33,95,21,94,88,84,92,55,60,19,34,5,69,14,6,9,87,50,7,77,64,35,7,35,73,73,40,44,28,62,32,26,41,22,27,19,10,42,14,83,80,80,52,18,86,38,48,84,81,78,15,75,26,1,80,58,86,58,83,22,89,49,70,44,54,74,31,49,94,39,49,52,65,99,24,73,5,8,35,23,49,64,56,93,33,15,16,51,28,97,1,56,67,54,22,1,17,57,37,95,68,39,31,21,62,33,7,89,97,92,99,100,26,46,27,53,9,57,25,81,76,90,87,58,99,100,88,97,4,33,89,8,42,71,37,92,30,79,58,55,68,1,14,52,61,40,76,80,39,15,62,38,22,61,77,89,83,100,35,70,52,38,78,99,52,80,3,48,55,70,11,31,62,69,76,82,69,75,10,18,49,70,23,39,39,39,48,84,45,35,62,16,63,31,49,90,45,81,8,29,74,72,44,99,16,5,31,87,84,55,80,20,41,28,1,74,55,25,73,88,51,100,1,27,87,47,47,34,83,63,62,45,8,52,77,95,76,70,89,89,56,37,5,26,74,6,94,20,47,74,54,2,62,52,29,62,62,62,89,79,57,36,44,93,93,86,48,9,55,5,21,49,39,1,62,94,8,28,38,12,47,96,19,41,76,79,76,73,7,58,91,76,75,7,54,12,89,63,89,26,48,67,38,26,100,58,25,46,85,16,19,7,82,34,27,13,12,22,90,73,14,82,56,55,62,4,45,93,52,11,68,97,93,50,19,45,44,68,9,28,81,69,39,59,17,34,1,82,42,28,44,46,81,5,38,88,80,8,91,26,59,82,39,36,94,75,93,72,83,13,82,49,8,6,33,72,4,78,20,14,1
*/