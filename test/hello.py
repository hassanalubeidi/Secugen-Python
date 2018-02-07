import myModule
from flask import Flask
app = Flask(__name__)

@app.route('/')
def hello_world():
    print(myModule.fib(10))
    print(myModule.version())
    return "<strong>Hello world!</strong>"