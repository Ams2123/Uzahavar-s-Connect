# app.py
from flask import Flask, request, jsonify, render_template
import cv2
import numpy as np
from ultralytics import YOLO
import base64
import os

app = Flask(__name__)

# Load YOLO model
# model = YOLO('best.pt')  # Replace with your model path

def process_image(image_data,model):
    # Decode base64 image
    encoded_data = image_data.split(',')[1]
    nparr = np.frombuffer(base64.b64decode(encoded_data), np.uint8)
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    
    # Run inference
    results = model.predict(img)
    
    # Process results
    detections = []
    for r in results:
        boxes = r.boxes
        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0].tolist()
            conf = float(box.conf[0])
            cls = int(box.cls[0])
            name = model.names[cls]
            
            detections.append({
                'bbox': [x1, y1, x2, y2],
                'confidence': conf,
                'class': name
            })
    
    return detections

@app.route('/',methods=['GET'])
def home():
    global model
    plant = request.args.get('plant')
    if plant == "coconut":
        model = YOLO(r'coconut\best.pt')
    elif plant == "mango":
        model = YOLO(r'mango\best.pt')
    elif plant == "pest":
        model = YOLO(r'pest\best.pt')
    elif plant == "weed":
        model = YOLO(r'weed\best.pt')

    return render_template('index.html')

@app.route('/detect', methods=['POST'])
def detect():
    if 'image' not in request.json:
        return jsonify({'error': 'No image provided'}), 400
    
    image_data = request.json['image']
    detections = process_image(image_data,model)
    return jsonify({'detections': detections})

if __name__ == '__main__':
    app.run(debug=True)