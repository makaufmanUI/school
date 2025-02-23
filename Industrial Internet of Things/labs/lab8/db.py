"""
db.py

Defines the `DB` class, which handles the Google Sheets connection and data insertion.
"""
import gspread
from datetime import datetime



class DB:
    def __init__(self, creds: str, sheet: str) -> None:
        """
        Instantiates the DB object.

        ## Parameters
        - `creds`:  The path to the JSON credentials file.
        - `sheet`:  The name of the spreadsheet to connect to.
        """
        self.client = gspread.service_account(creds)
        self.sheet  = self.client.open(sheet).sheet1

    def get_datetime(self) -> tuple[str, str]:
        """
        Returns the current date in `YYYY-MM-DD` format,
        and the current time in `HH:MM:SS` format, as a tuple of strings.
        """
        dt = datetime.now()
        return (dt.strftime("%Y-%m-%d"), dt.strftime("%H:%M:%S"))

    def insert(self, data: list) -> None:
        """
        Inserts a list of data into the spreadsheet, beneath the header row.

        ## Parameters
        - `data`:  The list of data to insert. Date and time are automatically added.
        """
        self.sheet.insert_row(data, 2)

    def push(self, data: float) -> None:
        """
        Pushes the given data to the spreadsheet,
        along with the current date and current time.

        ## Parameters
        - `data`:  The data to push to the spreadsheet (temperature in this case).
        """
        self.insert([*self.get_datetime(), data])

