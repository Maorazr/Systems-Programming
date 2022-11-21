import sqlite3

import DTO


class Hats:
    def __init__(self, connection):
        self.connection = connection

    def insert(self, hat):
        self.connection.execute("""
            INSERT INTO Hats (id, topping, supplier, quantity) VALUES (?,?,?,?)
        """, [hat.id, hat.topping, hat.supplier, hat.quantity])

    def findSuppliersIds(self, top):
        cursor = self.connection.cursor()
        cursor.execute(""" 
        SELECT supplier FROM Hats WHERE topping = ?
        ORDER BY supplier """, [top])
        suppliers = [*cursor.fetchall()]
        suppliers.sort()
        if len(suppliers) >= 1:
            return suppliers[0]
        else:
            return -1

    def reduceAmount(self, supplier, top):
        self.connection.execute("""
            UPDATE Hats SET quantity = quantity -1 WHERE supplier = ? AND topping = ? 
        """, [supplier[0], top])

        self.connection.execute("""
            DELETE FROM Hats WHERE quantity = 0
        """)

    def getHatId(self, top, supId):
        c = self.connection.cursor()
        c.execute("""
            SELECT id FROM Hats WHERE topping = ? AND supplier = ?
        """, [top, supId[0]])
        return c.fetchone()[0]

class Suppliers:
    def __init__(self, connection):
        self.connection = connection

    def insert(self, supplier):
        self.connection.execute("""
            INSERT INTO Suppliers (id, name) VALUES (?, ?)
        
        """, [supplier.id, supplier.name])

    def getName(self, id):
        c = self.connection.cursor()
        c.execute("""
            SELECT name FROM Suppliers WHERE id = ?
        """, [id[0]])
        return c.fetchone()[0]


class Orders:
    def __init__(self, connection):
        self.connection = connection
        self.currId = 1

    def insert(self, order):
        self.connection.execute("""
            INSERT INTO Orders (id, location, hat) VALUES (?, ?, ?)
        """, [order.id, order.location, order.hat])
