int sum_to(int limit) {
    int sum = 0;

    for (int i = 1; i <= limit; i++) {
        sum += i;
    }

    return sum;
}

int main(void) {
    return sum_to(5);
}
