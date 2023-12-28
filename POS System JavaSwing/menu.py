# Menu Drink Dictionary

class Drink:
    drink_id : str
    name : str
    price : float
    
    def __init__(self, id, name_, price_) -> None:
        self.drink_id = id
        self.name = name_
        self.price = price_    


drinks = {
    "0001": Drink("0001", "Classic Black Milk Tea", 5),
    "0002": Drink("0002", "Classic Green Milk Tea", 5),
    "0003": Drink("0003", "Classic Oolong Milk Tea", 5),
    "0004": Drink("0004", "Honey Black Milk Tea", 5),
    "0005": Drink("0005", "Honey Green Milk Tea", 5),
    "0006": Drink("0006", "Honey Oolong Milk Tea", 5),
    "0007": Drink("0007", "Honey Black Tea", 5),
    "0008": Drink("0008", "Honey Green Tea", 5),
    "0009": Drink("0009", "Classic Coffee", 5),
    "0010": Drink("0010", "Classic Coffee with Ginger", 5),
    "0011": Drink("0011", "Classic Coffee with Hokkaido", 5),
    "0012": Drink("0012", "Classic Coffee with Okinawa", 5),
    "0013": Drink("0013", "Classic Coffee with Thai", 5),
    "0014": Drink("0014", "Classic Coffee with Taro", 5),
    "0015": Drink("0015", "Classic Coffee with Mango Green", 5),
    "0016": Drink("0016", "Classic Coffee with QQ Happy Family", 5),
    "0017": Drink("0017", "Classic Coffee with Matcha", 5),
    "0018": Drink("0018", "Brewed Tea Classic", 4),
    "0019": Drink("0019", "Brewed Tea Black", 4),
    "0020": Drink("0020", "Brewed Tea Green", 4),
    "0021": Drink("0021", "Brewed Tea Oolong", 4),
    "0022": Drink("0022", "Brewed Tea Wintermelon", 4),
    "0023": Drink("0023", "Brewed Tea Honey", 4),
    "0024": Drink("0024", "Fruit Tea Mango Green", 5),
    "0025": Drink("0025", "Fruit Tea Wintermelon Lemonade", 5),
    "0026": Drink("0026", "Fruit Tea Strawberry", 5),
    "0027": Drink("0027", "Fruit Tea Peach Kiwi", 5),
    "0028": Drink("0028", "Fruit Tea Kiwi Fruit", 5),
    "0029": Drink("0029", "Fruit Tea Mango & Passion Fruit", 5),
    "0030": Drink("0030", "Fruit Tea Tropical Fruit", 5),
    "0031": Drink("0031", "Fruit Tea Hawaii Fruit", 5),
    "0032": Drink("0032", "Fruit Tea Passion Fruit Orange and Grapefruit", 5),
    "0033": Drink("0033", "Fresh Milk Fresh Milk", 4),
    "0034": Drink("0034", "Fresh Milk Black", 4),
    "0035": Drink("0035", "Fresh Milk Green", 4),
    "0036": Drink("0036", "Fresh Milk Oolong", 4),
    "0037": Drink("0037", "Fresh Milk Wintermelon", 4),
    "0038": Drink("0038", "Fresh Milk Cocoa Lover", 5),
    "0039": Drink("0039", "Fresh Milk Family", 5),
    "0040": Drink("0040", "Fresh Milk Pearl", 5),
    "0041": Drink("0041", "Fresh Milk Pudding", 5),
    "0042": Drink("0042", "Fresh Milk Herb Jelly", 5),
    "0043": Drink("0043", "Fresh Milk Matcha", 5),
    "0044": Drink("0044", "Handmade Taro Ice Blended", 6),
    "0045": Drink("0045", "Oreo Ice Cream", 6),
    "0046": Drink("0046", "Milk Tea Ice Cream", 6),

}

# make .csv for drink dictionary
file = open("drink_dictionary.csv", "w") 

# headers
file.write("drink_id,name,price\n")
for drink in drinks.keys() :
    file.write(str(drinks[drink].drink_id) + ",")
    file.write(str(drinks[drink].name) + ",")
    file.write(str(drinks[drink].price) + "\n")

file.close()