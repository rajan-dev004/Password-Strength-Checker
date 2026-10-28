
#include <stdio.h>
#include <string.h>
#include <ctype.h> 

void check_password_strength(char password[]);
int  is_common_password(char password[]);
int  has_sequential_pattern(char password[]);

static const char *common_passwords[] = {
    "password", "password1", "password123",
    "123456", "12345678", "123456789", "1234567890",
    "abcd1234", "abcd@123", "abcd@1234",
    "qwerty", "qwerty123", "qwerty@123",
    "iloveyou", "welcome", "welcome1", "welcome@1",
    "admin", "admin123", "admin@123",
    "letmein", "monkey", "dragon", "master",
    "pass@123", "pass@1234",
    "abc@1234", "abc@123",
    "test@123", "test1234",
    NULL  
};


int is_common_password(char password[]) {
    int i, j;
    char lower[256];
    int len = strlen(password);

   
    for (i = 0; i < len && i < 255; i++)
        lower[i] = tolower((unsigned char)password[i]);
    lower[len < 255 ? len : 255] = '\0';

    for (i = 0; common_passwords[i] != NULL; i++) {
        if (strcmp(lower, common_passwords[i]) == 0)
            return 1;
    }
    return 0;
}


int has_sequential_pattern(char password[]) {
    int len = strlen(password);
    int i;
    int run;

    if (len < 4) return 0;

  
    run = 1;
    for (i = 1; i < len; i++) {
        char prev = tolower((unsigned char)password[i - 1]);
        char curr = tolower((unsigned char)password[i]);
        if ((isalpha((unsigned char)prev) && isalpha((unsigned char)curr) &&
             curr == prev + 1) ||
            (isdigit((unsigned char)prev) && isdigit((unsigned char)curr) &&
             curr == prev + 1)) {
            run++;
            if (run >= 4) return 1;
        } else {
            run = 1;
        }
    }

    
    run = 1;
    for (i = 1; i < len; i++) {
        char prev = tolower((unsigned char)password[i - 1]);
        char curr = tolower((unsigned char)password[i]);
        if ((isalpha((unsigned char)prev) && isalpha((unsigned char)curr) &&
             curr == prev - 1) ||
            (isdigit((unsigned char)prev) && isdigit((unsigned char)curr) &&
             curr == prev - 1)) {
            run++;
            if (run >= 4) return 1;
        } else {
            run = 1;
        }
    }

    return 0;
}


int main() {
    char password[256];
    int choice;

    while (1) {
        printf("  PASSWORD STRENGTH CHECKER   \n");
        printf("1. Check Password Strength\n");
        printf("2. Exit\n");
        printf("What do you want to do? ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

        if (choice == 1) {
            printf("\nType your password: ");
            scanf("%s", password);
            check_password_strength(password);

        } else if (choice == 2) {
            printf("\nYou Exited the program\n\n");
            break;

        } else {
            printf("\nInvalid input. Try 1 or 2.\n\n");
        }
    }

    return 0;
}


void check_password_strength(char password[]) {
    int length  = strlen(password);
    int upper   = 0, lower = 0, digit = 0, special = 0;
    int score   = 0;
    int i;
    char ch;
    int common, sequential;

    for (i = 0; i < length; i++) {
        ch = password[i];

        if (ch >= 'A' && ch <= 'Z') {
            upper++;
        } else if (ch >= 'a' && ch <= 'z') {
            lower++;
        } else if (ch >= '0' && ch <= '9') {
            digit++;
        } else {
            special++;
        }
    }


    printf("\nDetails of the password:\n");
    printf("Total characters  : %d\n", length);
    printf("Uppercase letters : %d\n", upper);
    printf("Lowercase letters : %d\n", lower);
    printf("Numbers           : %d\n", digit);
    printf("Special characters: %d\n", special);


    printf("\n--- Checking the requirements ---\n");

    if (length >= 8) {
        printf("  Good, your password is long enough (8+ characters).\n");
        score++;
    } else {
        printf("  Too short! It's only %d characters. Needs at least 8.\n", length);
    }

    if (upper > 0) {
        printf("  Nice, you have at least one uppercase letter.\n");
        score++;
    } else {
        printf("  No uppercase letter found. Try adding one like 'A' or 'B'.\n");
    }

    if (lower > 0) {
        printf("  Good, lowercase letters are present.\n");
        score++;
    } else {
        printf("  No lowercase letter found. Add something like 'a' or 'b'.\n");
    }

    if (digit > 0) {
        printf("  Great, you have a number in there.\n");
        score++;
    } else {
        printf("  No number found. Consider adding a digit like '3' or '7'.\n");
    }

    if (special > 0) {
        printf("  You have a special character too, nice touch!\n");
        score++;
    } else {
        printf("  No special character. Adding one like '@' or '#' helps a lot.\n");
    }

    common     = is_common_password(password);
    sequential = has_sequential_pattern(password);

    /* Penalise score for logical weaknesses */
    if (common)     score -= 2;
    if (sequential) score -= 1;
    if (score < 0)  score  = 0;

    printf("\n--- Logical Weakness Check ---\n");

    if (common) {
        printf("  WARNING: This is a very commonly used password and can be\n");
        printf("           easily guessed. Please choose something unique!\n");
    } else {
        printf("  Not found in the common passwords list. Good.\n");
    }

    if (sequential) {
        printf("  WARNING: Your password contains a sequential pattern (e.g.\n");
        printf("           'abcd', '1234'). These are easy to crack. Mix it up!\n");
    } else {
        printf("  No obvious sequential pattern detected. Good.\n");
    }

    printf("\n--- Result ---\n");
    printf("Your score: %d out of 5\n", score);

    if (common || sequential) {
        printf("Strength  : Weak (logically predictable despite meeting basic rules)\n");
    } else if (score == 5 && length >= 12) {
        printf("Strength  : Very Strong - excellent password!\n");
    } else if (score >= 4) {
        printf("Strength  : Strong - good job!\n");
    } else if (score == 3) {
        printf("Strength  : Moderate - could be better.\n");
    } else if (score == 2) {
        printf("Strength  : Weak - needs improvement.\n");
    } else {
        printf("Strength  : Very Weak - please change this password.\n");
    }


    if (score < 5 || common || sequential) {
        printf("\nHere's how you can make it stronger:\n");
        if (length < 8)
            printf("  -> Make it at least 8 characters long.\n");
        if (upper == 0)
            printf("  -> Add an uppercase letter (A to Z).\n");
        if (lower == 0)
            printf("  -> Add a lowercase letter (a to z).\n");
        if (digit == 0)
            printf("  -> Add a number (0 to 9).\n");
        if (special == 0)
            printf("  -> Add a special character like !, @, #, or $.\n");
        if (common)
            printf("  -> Avoid dictionary words and widely-known passwords.\n");
        if (sequential)
            printf("  -> Avoid sequences like 'abcd', 'wxyz', '1234', '9876'.\n");
    }

    printf("\n");
}
