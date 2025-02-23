"""
main.py

The main script for this lab.
"""
import asyncio
import datetime
from db import DB
from util import *
from adc import ADC
import pandas as pd
import streamlit as st


adc = ADC(input_channel = 0)
database = DB("creds.json","Lab8")


def button_callback(new_interval: int) -> None:
	# Update the session state variable update_interval with the value of the slider
    st.session_state.interval = new_interval
    st.success(f"Update interval changed successfully. New update interval: {st.session_state.interval} seconds")


async def update(p1, p2, p3, p4):
    while True:
        st.session_state.counter += 1           # update the counter
        temp = to_fahrenheit(T2(adc.read()))    # read temperature from ADC

        if st.session_state.counter % st.session_state.interval == 0:
            database.push(round(temp,3))        # push temperature to database
        
        # add the new data to the dataframe
        st.session_state.historical_data.loc[len(st.session_state.historical_data)] = [datetime.now(), temp]
        
        p1.write(f"**Temperature:** &nbsp; {round(temp,3)} °F")
        p2.write(f"**Current time:** &nbsp; {get_datetime_str()}")
        p3.line_chart(st.session_state.historical_data, x="Time", y="Temperature")
        p4.write(f"**Current interval:** &nbsp; {st.session_state.interval} seconds")
        
        await asyncio.sleep(1)



if __name__ == "__main__":
    if 'counter' not in st.session_state:
        st.session_state.counter = 0
    if 'interval' not in st.session_state:
        st.session_state['interval'] = 30
    if 'historical_data' not in st.session_state:
        st.session_state['historical_data'] = pd.DataFrame(columns=['Time', 'Temperature'])
    
    st.set_page_config(page_title="Temperature",page_icon=":thermometer:",layout="centered")
    with open("style.css") as f:      # load css file
        st.markdown(f"<style>{f.read()}</style>", unsafe_allow_html=True)

    st.markdown("# Thermistor temperature")
    st.markdown("---")
    st.write("")
    st.markdown("## Latest Data")
    st.write("")

    with st.container():
        current_datetime = st.empty()
        latest_temperature = st.empty()

    st.write("")
    st.write("")

    chart = st.empty()

    st.markdown("---")

    st.markdown("## Update Interval")
    st.markdown("Time between updates to the database.")

    current_interval = st.empty()
    update_interval = st.slider("Update Interval",min_value=30,max_value=300,value=st.session_state.interval,step=5,label_visibility="hidden")

    st.write("")

    confirmButton = st.button("Confirm", key=None, help=None, on_click=button_callback, args=(update_interval,))

    st.write("")
    st.markdown("---")

    asyncio.run(update(current_datetime, latest_temperature, chart))

