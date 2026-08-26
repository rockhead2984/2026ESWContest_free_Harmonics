from flask import Flask
from flask_socketio import SocketIO
import mysql.connector
import paho.mqtt.client as mqtt
import threading

app = Flask(__name__)
socketio = SocketIO(app)

def get_db():
    return mysql.connector.connect(
        host="localhost",
        user="root",
        password="psj2984@",
        database="escalator"
    )

HTML = """
<!DOCTYPE html>
<html>
<head>
    <title>에스컬레이터 모니터링</title>
    <script src="https://cdn.socket.io/4.0.0/socket.io.min.js"></script>
    <style>
        body { font-family: Arial; background: #1a1a2e; color: white; text-align: center; }
        h1 { color: #e94560; margin-top: 50px; }
        .card { background: #16213e; border-radius: 15px; padding: 30px; margin: 20px auto; width: 300px; }
        .distance { font-size: 60px; }
        .status { font-size: 24px; }
        table { margin: auto; border-collapse: collapse; width: 80%; }
        td, th { padding: 10px; border: 1px solid #333; }
        .normal { color: #4ecca3; }
        .danger { color: #e94560; }
    </style>
</head>
<body>
    <h1>🚀 에스컬레이터 혼잡도 모니터링</h1>
    <div class="card">
        <div class="distance" id="distance">-- mm</div>
        <div class="status" id="status">대기중</div>
    </div>
    <h2>최근 데이터</h2>
    <table id="table">
        <tr><th>ID</th><th>거리 (mm)</th><th>시간</th></tr>
    </table>
    <script>
        var socket = io();
        socket.on('update', function(data) {
            document.getElementById('distance').innerText = data.distance + ' mm';
            if (data.distance < 300) {
                document.getElementById('status').innerText = '⚠️ 혼잡';
                document.getElementById('status').className = 'status danger';
                document.getElementById('distance').className = 'distance danger';
            } else {
                document.getElementById('status').innerText = '✅ 여유';
                document.getElementById('status').className = 'status normal';
                document.getElementById('distance').className = 'distance normal';
            }
            var table = document.getElementById('table');
            var row = table.insertRow(1);
            row.innerHTML = '<td>' + data.id + '</td><td>' + data.distance + '</td><td>' + data.timestamp + '</td>';
            if (table.rows.length > 21) table.deleteRow(21);
        });
    </script>
</body>
</html>
"""

@app.route('/')
def index():
    return HTML

def on_connect_mqtt(client, userdata, flags, rc):
    client.subscribe("escalator/distance")

def on_message_mqtt(client, userdata, msg):
    try:
        data = msg.payload.decode().strip()
        distance = int(data)
        db = get_db()
        cursor = db.cursor()
        cursor.execute("INSERT INTO sensor_data (distance) VALUES (%s)", (distance,))
        db.commit()
        cursor.execute("SELECT id, timestamp FROM sensor_data ORDER BY id DESC LIMIT 1")
        row = cursor.fetchone()
        db.close()
        socketio.emit('update', {
            'distance': distance,
            'id': row[0],
            'timestamp': str(row[1])
        })
    except ValueError:
        pass

def mqtt_thread():
    client = mqtt.Client()
    client.on_connect = on_connect_mqtt
    client.on_message = on_message_mqtt
    client.connect("192.168.123.100", 1883, 60)
    client.loop_forever()

if __name__ == '__main__':
    t = threading.Thread(target=mqtt_thread)
    t.daemon = True
    t.start()
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)