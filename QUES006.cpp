#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 20
#define MAX_BILLS 50
#define MAX_COMPONENTS 3

typedef enum
{
    PACKAGED,
    COOKED,
    COMBO
} ItemType;

typedef struct
{
    int day, month, year;
} Date;

typedef struct
{
    int hour, minute;
} PrepTime;

typedef struct
{
    int itemID[MAX_COMPONENTS];
    int quantity[MAX_COMPONENTS];
    int count;
} Combo;

typedef union
{
    Date expiry;
    PrepTime prepared;
    Combo combo;
} ItemDetails;

typedef struct
{
    int id;
    char name[30];
    ItemType type;
    float price;
    int stock;
    ItemDetails details;
} Item;

typedef struct
{
    int billNo;
    char itemName[30];
    int quantity;
    float revenue;
} Bill;

Item inventory[MAX_ITEMS] =
{
    {101, "Chips", PACKAGED, 20, 10,
        {.expiry = {22, 11, 2026}}},

    {102, "Juice", PACKAGED, 30, 5,
        {.expiry = {23, 11, 2026}}},

    {201, "Sandwich", COOKED, 50, 8,
        {.prepared = {10, 30}}},

    {202, "Tea", COOKED, 15, 20,
        {.prepared = {11, 0}}},

    {301, "Combo Meal", COMBO, 60, 0,
        {.combo = {{201, 202}, {1, 1}, 2}}}
};

Bill bills[MAX_BILLS];

int itemCount = 5;
int billCount = 0;
int nextBill = 1;

int findItem(int id)
{
    for (int i = 0; i < itemCount; i++)
    {
        if (inventory[i].id == id)
            return i;
    }
    return -1;
}

int availableStock(int index)
{
    Item *p = &inventory[index];

    if (p->type != COMBO)
        return p->stock;

    int available = 999;

    for (int i = 0; i < p->details.combo.count; i++)
    {
        int k = findItem(p->details.combo.itemID[i]);

        if (k == -1)
            return 0;

        int possible = inventory[k].stock /
            p->details.combo.quantity[i];

        if (possible < available)
            available = possible;
    }

    return available;
}

void displayInventory(void)
{
    printf("\n--- CANTEEN INVENTORY ---\n");

    for (int i = 0; i < itemCount; i++)
    {
        printf("%d  %s  Rs.%.2f  Stock:%d\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].price,
               availableStock(i));
    }
}

void updateStock(int index, int qty)
{
    Item *p = &inventory[index];

    if (p->type != COMBO)
    {
        p->stock -= qty;
    }
    else
    {
        for (int i = 0; i < p->details.combo.count; i++)
        {
            int k = findItem(p->details.combo.itemID[i]);

            inventory[k].stock -=
                p->details.combo.quantity[i] * qty;
        }
    }
}

void recordSale(int id, int qty)
{
    int index = findItem(id);

    if (index == -1 || qty <= 0)
    {
        printf("Invalid item or quantity!\n");
        return;
    }

    if (billCount >= MAX_BILLS)
    {
        printf("Bill storage full!\n");
        return;
    }

    if (qty > availableStock(index))
    {
        printf("Insufficient stock for %s!\n",
               inventory[index].name);
        return;
    }

    float total = inventory[index].price * qty;

    updateStock(index, qty);

    bills[billCount].billNo = nextBill++;

    strcpy(bills[billCount].itemName,
           inventory[index].name);

    bills[billCount].quantity = qty;
    bills[billCount].revenue = total;

    billCount++;

    printf("Sale successful: %s x %d = Rs.%.2f\n",
           inventory[index].name, qty, total);
}

int daysUntilExpiry(Date d)
{
    struct tm expiry = {0};
    struct tm today = {0};

    time_t now = time(NULL);
    struct tm *current = localtime(&now);

    if (current == NULL)
        return -999;

    today.tm_mday = current->tm_mday;
    today.tm_mon = current->tm_mon;
    today.tm_year = current->tm_year;

    expiry.tm_mday = d.day;
    expiry.tm_mon = d.month - 1;
    expiry.tm_year = d.year - 1900;

    time_t t1 = mktime(&today);
    time_t t2 = mktime(&expiry);

    return (int)(difftime(t2, t1) / 86400.0);
}

void expiryWarning(void)
{
    printf("\n--- EXPIRY WARNINGS ---\n");

    for (int i = 0; i < itemCount; i++)
    {
        if (inventory[i].type == PACKAGED)
        {
            Date d = inventory[i].details.expiry;
            int days = daysUntilExpiry(d);

            if (days >= 0 && days <= 2)
            {
                printf("%s expires in %d day(s)!\n",
                       inventory[i].name, days);
            }
        }
    }
}

void sortBills(void)
{
    Bill temp;

    for (int i = 0; i < billCount - 1; i++)
    {
        for (int j = 0; j < billCount - i - 1; j++)
        {
            if (bills[j].revenue < bills[j + 1].revenue)
            {
                temp = bills[j];
                bills[j] = bills[j + 1];
                bills[j + 1] = temp;
            }
        }
    }
}

void displaySummary(void)
{
    sortBills();

    float total = 0;

    printf("\n--- DAILY BILL SUMMARY ---\n");

    for (int i = 0; i < billCount; i++)
    {
        printf("Bill %d | %s | Qty: %d | Rs.%.2f\n",
               bills[i].billNo,
               bills[i].itemName,
               bills[i].quantity,
               bills[i].revenue);

        total += bills[i].revenue;
    }

    printf("Total Revenue: Rs.%.2f\n", total);
}

int main(void)
{
    printf("COLLEGE CANTEEN INVENTORY AND BILLING\n");

    displayInventory();

    printf("\n--- SALES ---\n");

    recordSale(101, 2);
    recordSale(301, 2);
    recordSale(102, 8);
    recordSale(201, 1);

    expiryWarning();

    displayInventory();

    displaySummary();

    return 0;
}