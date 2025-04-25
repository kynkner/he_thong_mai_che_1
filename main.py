from flask import Flask, request, render_template, jsonify
import json
from datetime import datetime

app = Flask(__name__)
DATA_FILE = "data.json"

@app.route('/')
def index():
    return render_template('index.html')

# @app.route('/data', methods=['POST'])
# def receive_data():
#     content = request.json
#     entry = {
#         "timestamp": datetime.now().isoformat(),
#         "rain": content['rain'],
#         "light": content['light'],
#         "roof": content['roof']
#     }

#     try:
#         with open(DATA_FILE, 'r') as f:
#             data = json.load(f)
#     except:
#         data = []

#     data.append(entry)
#     with open(DATA_FILE, 'w') as f:
#         json.dump(data, f)

#     return jsonify({"status": "success"})1

@app.route('/data', methods=['POST'])
def receive_data():
    try:
        content = request.json
        print("Received from ESP32:", content)  # In log dữ liệu nhận được

        # Lưu vào file data.json
        with open('data.json', 'r') as f:
            data = json.load(f)

        # Thêm dữ liệu mới vào danh sách
        data.append(content)

        # Ghi lại dữ liệu vào file
        with open('data.json', 'w') as f:
            json.dump(data, f, indent=4)

        return jsonify({"status": "success", "message": "Data received"}), 200
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({"status": "error", "message": "Failed to process data"}), 400

    

@app.route('/get-data')
def get_data():
    try:
        with open('data.json', 'r') as f:
            try:
                data = json.load(f)
            except json.JSONDecodeError:
                data = []
    except FileNotFoundError:
        data = []

    return jsonify(data)


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
