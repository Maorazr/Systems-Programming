import sqlite3
import os
from Repository import Repository
import DTO
import sys


def main():
    databaseexisted = os.path.isfile(sys.argv[4])
    dbcon = sqlite3.connect(sys.argv[4])
    repo = Repository()

    with dbcon:
        if not databaseexisted:
            repo.create_table()

    # with open('config.txt') as configfile:
    with open(sys.argv[1]) as configfile:
        lines = configfile.readlines()
        for i in range(len(lines)):
            lines[i] = lines[i].strip()

        num_of_rows = lines[0].split(",")
        for i in range(1, int(num_of_rows[0]) + 1):
            row = lines[i].split(",")
            hat = DTO.Hat(int(row[0]), row[1], int(row[2]), int(row[3]))
            repo.hat.insert(hat)

        for i in range(int(num_of_rows[0]) + 1, int(num_of_rows[0]) + 1 + int(num_of_rows[1])):
            row = lines[i].split(",")
            supplier = DTO.Supplier(int(row[0]), row[1])
            repo.supplier.insert(supplier)

    with open(sys.argv[2]) as ordersfile, open(sys.argv[3], 'w') as output:
        orders = ordersfile.readlines()
        for i in range(len(orders)):
            orders[i] = orders[i].strip()

        for i in range(len(orders)):
            order = orders[i].split(",")
            suppId = repo.hat.findSuppliersIds(order[1])
            if suppId != -1:
                topId = repo.hat.getHatId(order[1], suppId)
                ord = DTO.Order(i + 1, order[0], topId)
                repo.order.insert(ord)
                repo.hat.reduceAmount(suppId, order[1])
                name = repo.supplier.getName(suppId)
                output.write("{},{},{}\n".format(order[1], name, order[0]))

    repo.close()


if __name__ == '__main__':
    main()
