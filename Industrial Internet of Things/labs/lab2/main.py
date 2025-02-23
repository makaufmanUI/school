"""
09/14/2022
Industrial Internet of Things
Lab 2: Using Google Sheets as a Cloud Database

Write the date, time, and random float between 0 and 100 to the spreadsheet
at a rate of 1 data entry per 30 seconds, with the newest data being on top.

One hour's worth of data, with the format:
Date (YYYY-MM-DD)     Time (HH:MM:SS)     Random Float (1.234)
"""
import time
import numpy
import gspread
from datetime import datetime


client = gspread.service_account("creds.json")
sheet  = client.open("Lab2").sheet1


def get_datetime() -> tuple[str, str]:
    """
    Returns the current date in `YYYY-MM-DD` format,
    and the current time in `HH:MM:SS` format, as a tuple of strings.
    """
    dt = datetime.now()
    return (dt.strftime("%Y-%m-%d"), dt.strftime("%H:%M:%S"))


def get_float() -> float:
    """
    Returns a random float in the interval `[0, 100)` to 3 decimal places.
    """
    return round(numpy.random.rand() * 100, 3)


if __name__ == "__main__":
    for i in range(120):
        data = [*get_datetime(), get_float()]
        print(f"Iteration # {i+1}")
        print(">>  Date: {}    Time: {}    Float: {}\n".format(*data))
        sheet.insert_row(data, 2)
        time.sleep(30)
    
    print("\nDone.")

