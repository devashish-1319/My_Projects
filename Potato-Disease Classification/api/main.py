from fastapi import FastAPI, File, UploadFile
from fastapi.middleware.cors import CORSMiddleware
import uvicorn
import numpy as np
from io import BytesIO
from PIL import Image
import tensorflow as tf

app = FastAPI()

origins = [
    "http://localhost",
    "http://localhost:3000",
]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

#MODEL = tf.keras.models.load_model("/Users/devadeva/Desktop/MNIT/Machine Learning/Potato-Disease Classification/Models/1")
from keras.layers import TFSMLayer
MODEL = TFSMLayer(
    "/Users/devadeva/Desktop/MNIT/Machine Learning/Potato-Disease Classification/Models/1",
    call_endpoint="serving_default"  # this is usually correct
)

CLASS_NAMES = ["Early Blight", "Late Blight", "Healthy"] #naming should be consistent to what is there in the jupyter notebook


@app.get("/ping")
async def ping():
    return "Hello, I am alive"


#def read_file_as_image(data) -> np.ndarray:
#    image = np.array(Image.open(BytesIO(data)))
#    return image
def read_file_as_image(data) -> np.ndarray:
    image = Image.open(BytesIO(data)).resize((256, 256))  # Resize if needed
    image = np.array(image).astype(np.float32) / 255.0     # Convert to float32 and normalize
    return image



@app.post("/predict")
async def predict(
        file: UploadFile = File(...)
):
    image = read_file_as_image(await file.read())
    img_batch = np.expand_dims(image, 0)

    #predictions = MODEL.predict(img_batch)
    predictions = MODEL(img_batch, training=False)
    if isinstance(predictions,dict):
        predictions=list(predictions.values())[0]

    predicted_class = CLASS_NAMES[np.argmax(predictions[0])]
    confidence = np.max(predictions[0])
    return {
        'class': predicted_class,
        'confidence': float(confidence)
    }


if __name__ == "__main__":
    uvicorn.run(app, host='localhost', port=8000)