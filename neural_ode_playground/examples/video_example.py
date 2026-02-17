import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
import torchvision.transforms as transforms
from torchvision.datasets import ImageFolder
import timm

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

import time

start_time = time.time()
n_proc = 12

torch.set_num_threads(n_proc)
torch.set_num_interop_threads(1)


class PlayingCardDataset(Dataset):
    def __init__(self, data_dir, transform = None):
        self.data = ImageFolder(data_dir, transform = transform)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        return self.data[idx]
    
    @property
    def classes(self):
        return self.data.classes

dataset = PlayingCardDataset(
    data_dir = "train"
)

# print(len(dataset))
# print(dataset[0])

image, label = dataset[6000]
# print(image)
# print(label)

# get a dictionary associating target values with folder names
data_dir = "train"
target_to_class = {v: k for k, v in ImageFolder(data_dir).class_to_idx.items()}
# print(target_to_class)

transform = transforms.Compose([
    transforms.Resize((128, 128)),
    transforms.ToTensor(),
])

data_dir = "train"
dataset = PlayingCardDataset(data_dir, transform)

image, label = dataset[100]
# print(image.shape)

# iterate over dataset
for image, label in dataset:
    break

# create and iterate over dataloader
dataloader = DataLoader(dataset, batch_size = 32, shuffle = True)
for images, labels in dataloader:
    break

# create pytorch model
class SimpleCardClassifier(nn.Module):
    def __init__(self, num_classes = 53):
        super(SimpleCardClassifier, self).__init__()
        # Where we define all the parts of the model
        self.base_model = timm.create_model("efficientnet_b0", pretrained = True)
        self.features = nn.Sequential(*list(self.base_model.children())[:-1])
        
        enet_out_size = 1280
        # Make a classifier
        self.classifier = nn.Linear(enet_out_size, num_classes)

    def forward(self, x):
        # Connect these parts and return the output
        x = self.features(x)
        output = self.classifier(x)
        return output

model = SimpleCardClassifier(num_classes = 53)
# print(str(model)[:500])
# print(model(images).shape)

# Loss function
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr = 0.001)

# setup datasets
transform = transforms.Compose([
    transforms.Resize((128, 128)),
    transforms.ToTensor(),
])

train_folder = "train"
valid_folder = "valid"
test_folder  = "test"

train_dataset = PlayingCardDataset(train_folder, transform = transform)
valid_dataset = PlayingCardDataset(valid_folder, transform = transform)
test_dataset  = PlayingCardDataset(test_folder,  transform = transform)

train_loader = DataLoader(train_dataset, batch_size = 32, shuffle = True , num_workers = n_proc)
valid_loader = DataLoader(valid_dataset, batch_size = 32, shuffle = False, num_workers = n_proc)
test_loader  = DataLoader(test_dataset,  batch_size = 32, shuffle = False, num_workers = n_proc)

num_epoch = 5
train_losses, valid_losses = [], []

for epoch in range(num_epoch):
    # Set the model to train
    model.train()
    running_loss = 0.0
    for images, labels in train_loader:
        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()
        running_loss += loss.item() * images.size(0)
    train_loss = running_loss / len(train_loader.dataset)
    train_losses.append(train_loss)

    # validation phase
    model.eval()
    running_loss = 0.0
    with torch.no_grad():
        for images, labels in valid_loader:
            outputs = model(images)
            loss = criterion(outputs, labels)
            running_loss += loss.item() * images.size(0)
    valid_loss = running_loss / len(valid_loader.dataset)
    valid_losses.append(valid_loss)

    # Print epoch stats
    print(f"Epoch {epoch + 1}/{num_epoch} - Train loss: {train_loss}, Validation loss: {valid_loss}")

end_time = time.time()
elapsed_time = end_time - start_time
print(f"Time: {elapsed_time}")
