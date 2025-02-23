"""
googlesheet.py

Defines the `GoogleSheet` class for easily pushing new data to a spreadsheet.
"""
import gspread
from datetime import datetime



class GoogleSheet:
    def __init__(self, sheet_name: str) -> None:
        self.client = gspread.service_account("creds.json")
        self.sheet  = self.client.open(sheet_name).sheet1

    def get_datetime(self) -> tuple[str, str]:
        """
        Returns the current date in `YYYY-MM-DD` format,
        and the current time in `HH:MM:SS` format, as a tuple of strings.
        """
        dt = datetime.now()
        return (dt.strftime("%Y-%m-%d"), dt.strftime("%H:%M:%S"))
    
    def push_data(self, distance: float, motor_speed: int, proximity_limits: tuple[float, float, float, float]) -> None:
        """
        Pushes a new row of data to the Google Sheet.
        """
        data = [*self.get_datetime(), distance, motor_speed, proximity_limits]
        self.sheet.insert_row(data, 2)

