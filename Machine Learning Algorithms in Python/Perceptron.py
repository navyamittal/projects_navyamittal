import numpy as np
import pandas as pd
from typing import Tuple
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

# read_data, get_df_shape, data_split are the same as HW2-kNN
def read_data(filename: str) -> pd.DataFrame:
    d = pd.read_csv(filename)
    df = pd.DataFrame(data=d)
    return df

def get_df_shape(df: pd.DataFrame) -> Tuple[int, int]:
    return df.shape

def data_split(
    features: pd.DataFrame, label: pd.Series, test_size: float
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    X_train, X_test, y_train, y_test = train_test_split(features, label, test_size=test_size)
    return X_train, y_train, X_test, y_test


def extract_features_label(df: pd.DataFrame) -> Tuple[pd.DataFrame, pd.Series]:
    # Filter the dataframe to include only Setosa and Virginica rows
    # Extract the required features and labels from the filtered dataframe
    ########################
    ## Your Solution Here ##
    ########################
    # Define a dictionary to map flower species to labels
    label_mapping = {"Setosa": 0, "Virginica": 1}
    
    # Filter the DataFrame to include only the species of interest
    filtered_df = df[df['variety'].isin(label_mapping.keys())]
    
    # Extract the selected features (sepal.length and sepal.width)
    features_df = filtered_df[['sepal.length', 'sepal.width']]
    
    # Map the species to labels and create a Series for labels
    labels = filtered_df['variety'].map(label_mapping)
    
    return features_df, labels

class Perceptron:
    def __init__(self, learning_rate=0.01, epochs=1000):
        self.learning_rate = learning_rate
        self.epochs = epochs
        self.activation = self._unit_step_func
        self.weights = None
        self.bias = None
        self.error_list = []  # List to store errors at each epoch

    # def fit(self, X, y):
    #     """
    #     Train the perceptron on the given input features and target labels.
    #     You need to do following steps:
    #     1. Initialize the weights and bias (you can initialize both to 0)
    #     2. Calculate the linear output (Z) of the perceptron for all the points in X
    #     3. Apply the activation function to Z and get the predictions (Y_hat)
    #     4. Calculate the weight update rule for the perceptron and update the weights and bias
    #     5. Repeat steps 2-4 for 'epochs' number of times
    #     6. Return the final weights and bias
    #     Args:
    #         X (array-like): The input features.
    #         y (array-like): The target labels.

    #     Returns:
    #         weights (array-like): Learned weights.
    #         bias (float): Learned bias.
    #     """
    #     ########################
    #     ## Your Solution Here ##
    #     ########################
    #     # Initialize weights and bias
    #     num_samples, num_features = X.shape
    #     self.weights = np.zeros(num_features)
    #     self.bias = 0
        
    #     for _ in range(self.epochs):
    #         for i in range(num_samples):
    #             # Calculate the linear output (Z) of the perceptron
    #             linear_output = np.dot(X[i], self.weights) + self.bias
                
    #             # Apply the activation function to get predictions (Y_hat)
    #             y_hat = self.activation(linear_output)
                
    #             # Update the weights and bias based on the perceptron learning rule
    #             update = self.learning_rate * (y[i] - y_hat)
    #             self.weights += update * X[i]
    #             self.bias += update

    #     return self.weights, self.bias
    
    
    def fit(self, X, y):
        """
        Train the perceptron on the given input features and target labels.
        You need to do following steps:
        1. Initialize the weights and bias (you can initialize both to 0)
        2. Calculate the linear output (Z) of the perceptron for all the points in X
        3. Apply the activation function to Z and get the predictions (Y_hat)
        4. Calculate the weight update rule for the perceptron and update the weights and bias
        5. Accumulate the error at each epoch
        6. Repeat steps 2-5 for 'epochs' number of times
        7. Return the final weights, bias, and error_list
        Args:
            X (array-like): The input features.
            y (array-like): The target labels.

        Returns:
            weights (array-like): Learned weights.
            bias (float): Learned bias.
            error_list (list): List of errors at each epoch.
        """
        # Initialize weights and bias
        num_samples, num_features = X.shape
        self.weights = np.zeros(num_features)
        self.bias = 0
        
        for epoch in range(self.epochs):
            error = 0  # Initialize error for this epoch
            for i in range(num_samples):
                # Calculate the linear output (Z) of the perceptron
                linear_output = np.dot(X[i], self.weights) + self.bias
                
                # Apply the activation function to get predictions (Y_hat)
                y_hat = self.activation(linear_output)
                
                # Update the weights and bias based on the perceptron learning rule
                update = self.learning_rate * (y[i] - y_hat)
                self.weights += update * X[i]
                self.bias += update
                
                # Check for misclassification and accumulate errors
                if (y[i] - y_hat) != 0.0:
                    error += 1
            self.error_list.append(error)  # Append error for this epoch
            
        return self.weights, self.bias, self.error_list

    def predict(self, X):
        """
        Predict the labels for the given input features.

        Args:
            X (array-like): The input features.

        Returns:
            array-like: The predicted labels.
        """
        ########################
        ## Your Solution Here ##
        ########################
        # Calculate the linear output (Z) of the perceptron for the input features
        linear_output = np.dot(X, self.weights) + self.bias
        
        # Apply the activation function to get predictions (Y_hat)
        predictions = self.activation(linear_output)
        return predictions

    def _unit_step_func(self, x):
        """
        The unit step function, also known as the Heaviside step function.
        It returns 1 if the input is greater than or equal to zero, otherwise 0.

        Args:
            x (float or array-like): Input value(s) to the function.

        Returns:
            int or array-like: Result of the unit step function applied to the input(s).
        """
       # The unit step function returns 1 if input is greater than or equal to zero, else 0
        return np.where(x >= 0, 1, 0)


# Testing
if __name__ == "__main__":
    def accuracy(y_true, y_pred):
        accuracy = np.sum(y_true == y_pred) / len(y_true)
        return accuracy
    
    df = read_data("iris.csv")
    shape = get_df_shape(df)
    features, label = extract_features_label(df)
    X_train, y_train, X_test, y_test = data_split(features, label, 0.2)
    X_train, y_train, X_test, y_test = X_train.values, y_train.values, X_test.values, y_test.values
    
    # Define lists to store results for different hyperparameters
    epoch_values = [10, 50, 100]
    learning_rate_values = [0.7, 0.1, 0.01]
    
    # Create subplots for decision boundary plots
    fig, axs = plt.subplots(len(epoch_values), len(learning_rate_values), figsize=(15, 10))
    plt.subplots_adjust(wspace=0.4, hspace=0.4)
    
    for i, epochs in enumerate(epoch_values):
        for j, learning_rate in enumerate(learning_rate_values):
            p = Perceptron(learning_rate=learning_rate, epochs=epochs)
            p.fit(X_train, y_train)
            predictions = p.predict(X_test)
            
            accuracy_value = accuracy(y_test, predictions)
            print(f"Perceptron classification accuracy (Epochs={epochs}, Learning Rate={learning_rate}): {accuracy_value}")
            
            # Plot the decision boundary
            ax = axs[i, j]
            ax.scatter(X_train[:, 0], X_train[:, 1], marker="o", c=y_train)
            
            x0_1 = np.amin(X_train[:, 0])
            x0_2 = np.amax(X_train[:, 0])
            
            x1_1 = (-p.weights[0] * x0_1 - p.bias) / p.weights[1]
            x1_2 = (-p.weights[0] * x0_2 - p.bias) / p.weights[1]
            
            ax.plot([x0_1, x0_2], [x1_1, x1_2], "k")
            
            ymin = np.amin(X_train[:, 1])
            ymax = np.amax(X_train[:, 1])
            ax.set_ylim([ymin, ymax])
            ax.set_title(f"Epochs={epochs}, LR={learning_rate}, Accuracy={accuracy_value:.2f}")
    
    plt.show()
    
    
    
    
    plt.figure()
    plt.plot(range(p.epochs), p.error_list)
    plt.xlabel("Epochs")
    plt.ylabel("Errors")
    plt.title("Errors vs. Epochs")
    plt.show()