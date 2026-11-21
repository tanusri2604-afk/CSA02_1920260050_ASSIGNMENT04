#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NAME_LEN 20

typedef struct
{
    int systolic;
    int diastolic;
} BloodPressure;

typedef struct
{
    int day;
    int month;
    int year;
} VisitDate;

typedef struct
{
    int id;
    char name[NAME_LEN];
    int age;
    char phone[11];
    BloodPressure bp;
    VisitDate visitDate;
} Patient;

int addPatient(Patient patients[], int *count, Patient p)
{
    if (patients == NULL || count == NULL ||
        *count >= MAX_PATIENTS)
    {
        return 0;
    }

    if (strlen(p.phone) != 10)
    {
        return 0;
    }

    patients[*count] = p;
    (*count)++;

    return 1;
}

int main()
{
    Patient patients[MAX_PATIENTS];
    int count = 0;

    Patient p = {
        101,
        "Arun",
        25,
        "9876543210",
        {120, 80},
        {21, 11, 2026}
    };

    if (addPatient(patients, &count, p))
    {
        printf("Patient added successfully!\n");
        printf("ID: %d\n", patients[0].id);
        printf("Name: %s\n", patients[0].name);
        printf("Age: %d\n", patients[0].age);
        printf("Phone: %s\n", patients[0].phone);
        printf("Blood Pressure: %d/%d\n",
               patients[0].bp.systolic,
               patients[0].bp.diastolic);
        printf("Visit Date: %02d/%02d/%04d\n",
               patients[0].visitDate.day,
               patients[0].visitDate.month,
               patients[0].visitDate.year);
    }
    else
    {
        printf("Patient could not be added.\n");
    }

    return 0;
}