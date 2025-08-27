int countVisitsForAllBars(PersonP first) {
    if (!first) return EXIT_SUCCESS;

    PersonP temp = first;
    char currentBar[MAX];
    int count = 0;

    strcpy(currentBar, temp->barName);

    while (temp) {
        if (strcmp(temp->barName, currentBar) == 0) {
            count++;
        } else {
            // ispis prethodnog kafića
            printf("Kafić: %s -> broj ljudi: %d\n", currentBar, count);

            // reset brojača
            strcpy(currentBar, temp->barName);
            count = 1;
        }
        temp = temp->next;
    }

    // ispis zadnje grupe
    printf("Kafić: %s -> broj ljudi: %d\n", currentBar, count);

    return EXIT_SUCCESS;
}
