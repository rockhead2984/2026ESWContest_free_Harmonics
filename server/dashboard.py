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
        .card { background: #16213e; border-radius: 15px; padding: 30px; margin: 20px auto; width: 350px; }
        .distance { font-size: 60px; color: #4ecca3; }
        .status { font-size: 20px; margin-top: 10px; }
        .normal { color: #4ecca3; }
        .danger { color: #e94560; }
        table { margin: auto; border-collapse: collapse; width: 80%; }
        td, th { padding: 10px; border: 1px solid #333; }
    </style>
</head>
<body>
    <h1>🚀 에스컬레이터 혼잡도 모니터링</h1>
    <div class="card">
        <div>거리</div>
        <div class="distance" id="distance">-- mm</div>
        <div class="status" id="dist_status">대기중</div>
    </div>
    <div class="card">
        <div>모터 전류</div>
        <div class="distance" id="amp">-- mA</div>
        <div class="status" id="curr_status">대기중</div>
    </div>
    <h2>최근 데이터</h2>
    <table id="table">
        <tr><th>ID</th><th>거리 (mm)</th><th>전류 (mA)</th><th>시간</th></tr>
    </table>
    <script>
        var curr_timer = null;
        var socket = io();

        socket.on('update', function(data) {
            if (!data.distance) return;

            document.getElementById('distance').innerText = data.distance + ' mm';

            if (data.amp !== undefined) {
                document.getElementById('amp').innerText = data.amp.toFixed(1) + ' mA';
            }

            if (data.congestion == 1) {
                document.getElementById('dist_status').innerText = '⚠️ 혼잡합니다';
                document.getElementById('dist_status').className = 'status danger';
            } else {
                document.getElementById('dist_status').innerText = '✅ 여유';
                document.getElementById('dist_status').className = 'status normal';
            }

            if (data.current_flag == 1) {
                document.getElementById('curr_status').innerText = '⚠️ 양측 서기를 하십시오';
                document.getElementById('curr_status').className = 'status danger';
                if (curr_timer) clearTimeout(curr_timer);
                curr_timer = setTimeout(function() {
                    document.getElementById('curr_status').innerText = '✅ 정상';
                    document.getElementById('curr_status').className = 'status normal';
                }, 5000);
            }

            var table = document.getElementById('table');
            var row = table.insertRow(1);
            var amp_val = data.amp !== undefined ? data.amp.toFixed(1) : '--';
            row.innerHTML = '<td>' + data.id + '</td><td>' + data.distance + '</td><td>' + amp_val + ' mA</td><td>' + data.timestamp + '</td>';
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
    client.subscribe("escalator/data")

def on_message_mqtt(client, userdata, msg):
    try:
        data = msg.payload.decode().strip()
        print(f"data: {data}")  # 이거 추가
        if "DIST:" in data and len(data) > 10:
            parts = data.split(",")
            distance = int(parts[0].split(":")[1])
            cong = int(parts[1].split(":")[1])
            curr_flag = int(parts[2].split(":")[1])
            amp = int(parts[3].split(":")[1]) / 10.0
            
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
                'timestamp': str(row[1]),
                'congestion': cong,
                'current_flag': curr_flag,
                'amp': amp
            })
    except Exception as e:
        print(f"Error: {e}, data: {data}")

def mqtt_thread():
    client = mqtt.Client()
    client.on_connect = on_connect_mqtt
    client.on_message = on_message_mqtt
    client.connect("10.90.93.254", 1883, 60)
    client.loop_forever()

if __name__ == '__main__':
    t = threading.Thread(target=mqtt_thread)
    t.daemon = True
    t.start()
    socketio.run(app, host='0.0.0.0', port=5000, debug=False)