int choice;

do{
  printf("\n1. Play Game\n2. View High Scores\n3. Exit\n");
  printf("Choose: ");
  scanf("%d", &choice);
}

switch (choice) {
  case 1: playGame(); break;
  case 2: showScores(); break;
  case 3: printf("Goodbye!\n"); break;
  default: printf("Invalid choice!\n");
} while (choice != 3);
