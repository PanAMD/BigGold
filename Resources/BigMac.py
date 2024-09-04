import pandas as pd

# Function to load and preprocess data
def load_and_preprocess(file_path):
    big_mac_countries = [
        "ARG", "AUS", "BRA", "GBR", "CAN", "CHL", "CHN", "CZE", "DNK",
        "EGY", "HKG", "HUN", "IDN", "ISR", "JPN", "MYS", "MEX", "NZL",
        "NOR", "PER", "PHL", "POL", "RUS", "SAU", "SGP", "ZAF", "KOR",
        "SWE", "CHE", "TWN", "THA", "TUR", "ARE", "USA", "COL", "CRI",
        "PAK", "LKA", "UKR", "URY", "IND", "VNM", "GTM", "HND", "VEN",
        "NIC", "AZE", "BHR", "JOR", "KWT", "LBN", "MDA", "OMN",
        "QAT", "ROU", "EUZ"
    ]

    big_mac_data = pd.read_csv(file_path, na_values="#N/A")
    big_mac_data = big_mac_data.dropna(subset=["local_price"])  # Remove rows with missing local price
    big_mac_data['GDP_local'] = pd.to_numeric(big_mac_data['GDP_local'], errors='coerce')  # Convert GDP to number
    big_mac_data = big_mac_data.sort_values(by=['date', 'name'])  # Sort by date and name for easy reading

    big_mac_data['dollar_price'] = big_mac_data['local_price'] / big_mac_data['dollar_ex']

    return big_mac_data

# Function to calculate Big Mac Index
def calculate_big_mac_index(big_mac_data):
    big_mac_countries = [
        "ARG", "AUS", "BRA", "GBR", "CAN", "CHL", "CHN", "CZE", "DNK",
        "EGY", "HKG", "HUN", "IDN", "ISR", "JPN", "MYS", "MEX", "NZL",
        "NOR", "PER", "PHL", "POL", "RUS", "SAU", "SGP", "ZAF", "KOR",
        "SWE", "CHE", "TWN", "THA", "TUR", "ARE", "USA", "COL", "CRI",
        "PAK", "IND", "AZE", "BHR", "JOR", "KWT", "LBN", "MDA", "OMN",
        "QAT", "ROU", "EUZ"
    ]

    base_currencies = ["USD", "EUR", "GBP", "JPY", "CNY"]

    big_mac_index = big_mac_data[
        big_mac_data['iso_a3'].isin(big_mac_countries) & ~big_mac_data['dollar_price'].isna()
        ][['date', 'iso_a3', 'currency_code', 'name', 'local_price', 'dollar_ex', 'dollar_price']]

    for currency in base_currencies:
        # Create a series with the dollar price for the base currency
        base_currency_prices = big_mac_index.loc[
            big_mac_index['currency_code'] == currency,
            ['date', 'dollar_price']
        ].set_index('date')['dollar_price']

        # Map the base currency price to the corresponding date in big_mac_index
        big_mac_index[currency] = big_mac_index['date'].map(base_currency_prices)

        # Calculate the Big Mac Index
        big_mac_index[currency] = big_mac_index['dollar_price'] / big_mac_index[currency] - 1

    big_mac_index[base_currencies] = big_mac_index[base_currencies].round(5)

    return big_mac_index

# Function to save data to CSV
def save_to_csv(data, file_path):
    data.to_csv(file_path, index=False)
