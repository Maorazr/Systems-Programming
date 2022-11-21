import atexit
import sqlite3
import sys

import DAO
import DTO


class Repository:
    def __init__(self):
        self.connection = sqlite3.connect(sys.argv[4])
        self.hat = DAO.Hats(self.connection)
        self.supplier = DAO.Suppliers(self.connection)
        self.order = DAO.Orders(self.connection)

    def close(self):
        self.connection.commit()
        self.connection.close()

    def create_table(self):
        self.connection.executescript("""
        CREATE TABLE hats ( 
            id INTEGER PRIMARY KEY,
            topping TEXT NOT NULL,
            supplier INTEGER REFERENCES Supplier(id),
            quantity INTEGER NOT NULL
        );
            
        
        CREATE TABLE suppliers (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL
        );
            
        CREATE TABLE orders (
            id INTEGER PRIMARY KEY,
            location TEXT NOT NULL,
            hat INTEGER REFERENCES hats(id)
        );
        """)



