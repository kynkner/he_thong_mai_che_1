from flask import Flask, request, render_template, jsonify
import json
from datetime import datetime

app = Flask(__name__)
DATA_FILE = "data.json"

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data', methods=['POST'])
def receive_data():
    content = request.json
    entry = {
        "timestamp": datetime.now().isoformat(),
        "rain": content['rain'],
        "light": content['light'],
        "roof": content['roof']
    }

    try:
        with open(DATA_FILE, 'r') as f:
            data = json.load(f)
    except:
        data = []

    data.append(entry)
    with open(DATA_FILE, 'w') as f:
        json.dump(data, f)

    return jsonify({"status": "success"})

@app.route('/get-data')
def get_data():
    with open(DATA_FILE, 'r') as f:
        data = json.load(f)
    return jsonify(data)

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
