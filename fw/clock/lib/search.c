#include "search.h"

uint16_t uint16_binary_search(uint16_t *nums, uint16_t length, uint16_t target)
{
    uint16_t left = 0;
    uint16_t right = length - 1;

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