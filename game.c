#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#define KBC_API_URL "https://npp-mauve.vercel.app/ask"

#define MAX_QUES_LEN 300
#define MAX_OPTION_LEN 100
const char *COLOR_END = "\033[0m";
const char *RED = "\033[1;31m";
const char *GREEN = "\033[1;32m";
const char *YELLOW = "\033[1;33m";
const char *BLUE = "\033[1;34m";
const char *PINK = "\033[1;35m";
const char *AQUA = "\033[1;36m";

typedef struct
{
    char text[MAX_QUES_LEN];
    char options[4][MAX_OPTION_LEN];
    char correct_option;
    int timeout;
    int prize_money;
    int hidden[4];
} Question;

int read_questions(char *file_name, Question **questions);
void print_formatted_question(Question question);
void random_Question(Question *question, int no_of_questions)
{
    for (int i = no_of_questions - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Question temp = question[i];
        question[i] = question[j];
        question[j] = temp;
    }
}

void call_kbc_api(const Question *q)
{
    char command[1024];

    snprintf(command, sizeof(command),
             "curl -s -X POST %s "
             "-H \"Content-Type: text/plain\" "
             "-d \"%s\"",
             KBC_API_URL,
             q->text);

    printf("\n\n%sExpert says: %s", YELLOW, COLOR_END);
    fflush(stdout); // lable prints first

    system(command); // prin t API response directly

    printf("\n");
    printf("\n\nPress any key to continue...");
    _getch(); // any key from keybord
}

void play_game(Question *questions, int no_of_questions);
int use_lifeline(Question *question, int *lifeline);
int main()
{
    srand(time(NULL));

    Question *questions;
    int no_of_questions = read_questions("questions.txt", &questions);
    printf("\t\t %s lets play game %s", PINK, COLOR_END);

    play_game(questions, no_of_questions);

    free(questions);
    return 0;
}

int read_questions(char *file_name, Question **questions)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", file_name);
        exit(1);
    }

    char str[MAX_OPTION_LEN];
    int no_of_lines = 0;
    while (fgets(str, MAX_OPTION_LEN, file))
    {
        no_of_lines++;
    }
    int no_of_questions = no_of_lines / 8;
    *questions = (Question *)malloc(no_of_questions * sizeof(Question));
    if (*questions == NULL)
    {
        printf("Memory allocation failed\n");
        fclose(file);
        exit(1);
    }
    rewind(file);
    for (int i = 0; i < no_of_questions; i++)
    {
        fgets((*questions)[i].text, MAX_QUES_LEN, file);
        (*questions)[i].text[strcspn((*questions)[i].text, "\n")] = '\0';

        for (int j = 0; j < 4; j++)
        {
            fgets((*questions)[i].options[j], MAX_OPTION_LEN, file);
            (*questions)[i].options[j][strcspn((*questions)[i].options[j], "\n")] = '\0';
        }
        char option[10];
        fgets(option, 10, file);
        (*questions)[i].correct_option = option[0];

        char timeout[10];
        fgets(timeout, 10, file);
        (*questions)[i].timeout = atoi(timeout);

        char prize_money[10];
        fgets(prize_money, 10, file);
        (*questions)[i].prize_money = atoi(prize_money);
        for (int k = 0; k < 4; k++)
            (*questions)[i].hidden[k] = 0;
    }
    fclose(file);
    return no_of_questions;
}

void print_formatted_question(Question question)

{

    printf("\n\n%s%s%s", YELLOW, question.text, COLOR_END);

    for (int i = 0; i < 4; i++)
    {
        if (!question.hidden[i])
            printf("%s%c. %s%s\n", AQUA, ('A' + i), question.options[i], COLOR_END);
    }

    printf("\n%sHurry!! You have only %d Seconds to answer..%s", YELLOW, question.timeout, COLOR_END);
    printf("\n%sEnter your answer (A, B, C, or D) or L for lifeline: %s", GREEN, COLOR_END);
}

int use_lifeline(Question *question, int *lifeline)
{
    printf("\n\n%sAvailable Lifelines:%s", PINK, COLOR_END);
    if (lifeline[0])
        printf("\n%s1. Fifty-Fifty (50/50)%s", PINK, COLOR_END);
    if (lifeline[1])
        printf("\n%s2. Skip the Question%s", PINK, COLOR_END);
    if (lifeline[2])
        printf("\n%s3. ask the expert%s", PINK, COLOR_END);

    printf("\n%sChoose a lifeline or 0 to return: %s", PINK, COLOR_END);

    char ch = _getch();

    printf("%c", ch);

    switch (ch)
    {
    case '1':
        if (!lifeline[0])
            return 0;

        lifeline[0] = 0;
        int removed = 0;

        while (removed < 2)
        {
            int idx = rand() % 4;
            if (('A' + idx) != question->correct_option && question->hidden[idx] == 0)
            {
                question->hidden[idx] = 1;
                removed++;
            }
        }
        return 0;
    case '2':
        if (lifeline[1])
        {
            lifeline[1] = 0;
            return 2;
        }
        return 0;
    case '3':
        if (!lifeline[2])
            return 0;

        lifeline[2] = 0;
        call_kbc_api(question);
        return 3;
        break;
    default:
        return 0;
    }
    return 0;
}

char timed_input(int timeout_sec, int *timed_out)
{
    time_t start = time(NULL);
    *timed_out = 0;

    while (1)
    {
        if (_kbhit())
        {
            return _getch();
        }

        if (difftime(time(NULL), start) >= timeout_sec)
        {
            *timed_out = 1;
            return '\0';
        }
    }
}

void play_game(Question *questions, int no_of_questions)
{
    random_Question(questions, no_of_questions);

    int money_won = 0;
    int lifeline[3] = {1, 1, 1};
    for (int i = 0; i < no_of_questions; i++)
    {

        print_formatted_question(questions[i]);

        int timed_out = 0;
        char ch = timed_input(questions[i].timeout, &timed_out);

        if (timed_out)
        {
            printf("%s\n\nTime out!!!!!%s\n", RED, COLOR_END);
            break;
        }

        printf("%c", ch);
        ch = toupper(ch);

        if (ch == 'L')
        {
            int value = use_lifeline(&questions[i], lifeline);
            if (value != 2)
            {
                i--; // You need to decrement i before continue so the same question repeats:
                continue;
            }
        }

        if (ch == questions[i].correct_option)
        {
            printf("%s\nCorrect!%s", GREEN, COLOR_END);
            money_won = questions[i].prize_money;
            printf("\n%sYou have won: Rs %d%s", BLUE, money_won, COLOR_END);
        }
        else
        {
            printf("%s\nWrong! Correct answer is %c.%s", RED, questions[i].correct_option, COLOR_END);
            break;
        }
    }
    printf("\n\n%sGame Over! Your total winnings are: Rs %d%s\n", BLUE, money_won, COLOR_END);
}
