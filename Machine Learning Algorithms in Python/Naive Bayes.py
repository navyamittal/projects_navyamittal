import numpy as np
import pandas as pd
from typing import Tuple
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

# read_data, get_df_shape, data_split are the same as HW3
def read_data(filename: str) -> pd.DataFrame:
    d = pd.read_csv(filename)
    df = pd.DataFrame(data=d)
    return df

class GaussianNaiveBayes:
    def __init__(self, eps=1e-6):
        self.classes = None
        self.mean = None
        self.var = None
        self.priors = None
        self.eps = eps  # small constant to avoid division by zero

    def fit(self, X: np.ndarray, y: np.ndarray):
        """
        Train the Gaussian Naive Bayes model. Calculate the mean, variance, and prior probabilities for each class.

        Parameters:
        X : np.ndarray
            The training features
        y : np.ndarray
            The training labels
            
        Returns:
        mean : np.ndarray
              The mean for each class
        var : np.ndarray
              The variance for each class
        priors : np.ndarray
              The prior probabilities for each class
        """
        ########################
        ## Your Solution Here ##
        ########################
        # Get the unique classes in the target variable (y)
        unique_classes = np.unique(y)
        self.classes = unique_classes
        # Initialize arrays to store mean, variance, and prior for each class
        mean = np.zeros((len(unique_classes), X.shape[1]))
        var = np.zeros((len(unique_classes), X.shape[1]))
        priors = np.zeros(len(unique_classes))
        
        # Calculate mean and variance for each class
        for i, cls in enumerate(unique_classes):
            X_cls = X[y == cls]  # Subset of X for the current class
            mean[i] = np.mean(X_cls, axis=0)
            var[i] = np.var(X_cls, axis=0)
            
            # Calculate the prior probability for the current class
            priors[i] = len(X_cls) / len(y)
        
        self.mean = mean
        self.var = var
        self.priors = priors
        
        return mean, var, priors

    def gaussian_probability(self, x: np.ndarray, mu: np.ndarray, sigma2: np.ndarray) -> np.ndarray:
        """
        Compute the Gaussian probability.

        Parameters:
        x : np.ndarray
            The input features
        mu : np.ndarray
            The mean values
        sigma2 : np.ndarray
            The variance values

        Returns:
        np.ndarray
            The Gaussian probabilities
        """
        ########################
        ## Your Solution Here ##
        ########################
        # Ensure that x, mu, and sigma2 have the same shape
        #assert x.shape == mu.shape == sigma2.shape
        
        # Calculate the exponent part of the Gaussian distribution
        exponent = -0.5 * ((x - mu) ** 2) / sigma2
        
        # Calculate the normalization factor
        normalization = 1.0 / np.sqrt(2 * np.pi * sigma2)
        
        # Compute the Gaussian probability using the PDF formula
        probabilities = normalization * np.exp(exponent)
        
        return probabilities

    def predict(self, X: np.ndarray) -> np.ndarray:
        """
        Make predictions for given features using the trained Gaussian Naive Bayes model.

        Parameters:
        X : np.ndarray
            The features to predict

        Returns:
        np.ndarray
            The predicted classes
        """
        # Check if the model has been trained (self.mean and self.var are not None)
    
        # Ensure that X has the same number of features as the mean and variance arrays
        #assert X.shape[1] == self.mean.shape[1] == self.var.shape[1]
        
        # Initialize an array to store the predicted classes
        predictions = np.zeros(X.shape[0], dtype=int)
        
        # Iterate through each data point in X
        for i in range(X.shape[0]):
            # Calculate the class posteriors for the current data point
            posteriors = np.zeros(len(self.priors))
            for j in range(len(self.priors)):
                # Calculate the likelihood using Gaussian probability
                likelihood = self.gaussian_probability(X[i], self.mean[j], self.var[j])
                
                # Calculate the class posterior using the likelihood and prior
                posteriors[j] = np.log(np.prod(likelihood) * self.priors[j])
            
            # Select the class with the highest posterior as the predicted class
            predictions[i] = np.argmax(posteriors)
        
        return self.classes[predictions]


def visualization(df: pd.DataFrame):
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(35, 10))
    fig.suptitle("Data Visualization", fontsize=15)

    X = np.asarray(df.drop('variety', axis=1))
    x1, x2, x3, x4 = X.T[0], X.T[1], X.T[2], X.T[3]

    ax1.scatter(x1[:50], x2[:50], c='red')
    ax1.scatter(x1[50:100], x2[50:100], c='blue')
    ax1.scatter(x1[100:150], x2[100:150], c='green')
    ax1.set(xlabel='Sepal Length', ylabel='Sepal Width')

    ax2.scatter(x3[0:50], x4[0:50], c='red')
    ax2.scatter(x3[50:100], x4[50:100], c='blue')
    ax2.scatter(x3[100:150], x4[100:150], c='green')
    ax2.set(xlabel='Petal Length', ylabel="Petal Width")

    ax3.scatter(x1[:50]/x2[:50], x3[0:50]/x4[0:50], c='red')
    ax3.scatter(x1[50:100]/x2[50:100], x3[50:100]/x4[50:100], c='blue')
    ax3.scatter(x1[100:150]/x2[100:150], x3[100:150]/x4[100:150], c='green')
    ax3.set(xlabel='Sepal Length/Width', ylabel='Petal Length/Width')

    ax1.legend(['Iris-Setosa', 'Iris-Versicolor',
               'Iris-Virginica'], fontsize=15)
    ax2.legend(['Iris-Setosa', 'Iris-Versicolor',
               'Iris-Virginica'], fontsize=15)
    ax3.legend(['Iris-Setosa', 'Iris-Versicolor',
               'Iris-Virginica'], fontsize=15)

    plt.show()


# Testing
if __name__ == "__main__":
    def accuracy(y_true, y_pred):
        accuracy = np.sum(y_true == y_pred) / len(y_true)
        return accuracy

    # Read the data
    train_df = read_data("iris.csv")
    test_df = read_data("iris.csv")

    # Visualization for you to understand the data, you can comment it out
    # visualization(df)

    # Data preprocessing
    train_features = train_df.drop('variety', axis=1).values
    train_labels = train_df['variety'].values

    test_features = test_df.drop('variety', axis=1).values
    test_labels = test_df['variety'].values

    # Get the number of features
    num_features = train_features.shape[1]

    # Initialize and train the Naive Bayes classifier
    nb = GaussianNaiveBayes()
    nb.fit(train_features, train_labels)

    # Make predictions on the test set
    test_predictions = nb.predict(test_features)

    # Calculate accuracy
    accuracy = accuracy(test_labels, test_predictions)
    #print(test_predictions)
    #print(test_labels)
    print("Naive Bayes Classification Accuracy: {:.2f}%".format(
        accuracy * 100))
