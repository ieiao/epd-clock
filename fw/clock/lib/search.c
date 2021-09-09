#include "search.h"

int32_t int32_binary_search(int32_t *nums, int32_t length, int32_t target)
{
    int32_t left = 0;
    int32_t right = length - 1;

    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(nums[mid] == target)
            return mid;
        else if (nums[mid] < target)
            left = mid + 1;
        else if (nums[mid] > target)
            right = mid - 1;
    }
    return -1;
}