int sum_array(const int *values, int length) {
    int sum = 0;

    for (int i = 0; i < length; i++) {
        sum += values[i];
    }

    return sum;
}

int main(void) {
    int values[3] = {10, 20, 30};
    return sum_array(values, 3);
}
