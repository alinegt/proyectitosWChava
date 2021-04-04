# -*- coding: utf-8 -*-
# DO NOT CHANGE ABOVE LINE

'''
Requires VISA installed on Control PC
'keysight.com/find/iosuite'
Requires PyVISA to use VISA in Python
'http://PyVISA.sourceforge.net/PyVISA/'

# =============================================================================
Copyright © 2021 Keysight Technologies Inc. All rights reserved.

You have a royalty-free right to use, modify, reproduce and distribute this
example files (and/or any modified version) in any way you find useful,
provided that you agree that Keysight has no warranty, obligations or
liability for any Sample Application Files.
# =============================================================================

# =============================================================================
# Intro, general comments, and instructions
# =============================================================================

This example program is provided as-is and without support.  Keysight is not
responsible for modifications.

Tested with:

    Keysight IO Libraries Suite 18.1.26209.5
    Anaconda Python 3.8.5 64-bit
    PyVISA 1.11.1
    Windows 10 Enterprise, 64-bit
    InfiniiVision MSOX6004A with 7.35.2020120822 firmware

Compatibility notes:

    This script is for use with Keysight (and Agilent) InfiniiVision
    oscilloscopes.  It is expected to support models in the following families:

        - InfiniiVision 1000, 2000, 3000A/T, 4000, and 6000 X-series

Description of functionality:

    This script captures one or more waveforms from supported Keysight
    oscilloscopes and saves them to files in CSV and/or NumPy format.  Each
    file is given a unique file name representing the date and time of capture.

    The SYNC_METHOD user-defined constant controls whether execution waits for
    new triggers between screen captures or simply captures whatever is on
    screen without any synchronization.  The RunBitLow method generally works
    best under various conditions (e.g. very slow timescales).

Instructions:

    Read the comments in the User Inputs section below and set the constants
    there as needed for your application.

    Recommended familiarization steps:
        1. Connect a probe to channel 1 and hook the retractable probe tip onto
           the Probe Comp lug on the oscilloscope's front panel
        2. Press [Default Setup] and then [Autoscale]
        3. In the User Inputs section, set the SCOPE_VISA_ADDRESS to the one
           listed for your scope in Keysight Connection Expert
        4. Set the remaining User Inputs as follows:
             GENERAL_TIMEOUT = 15000
             TRIGGER_TIMEOUT = 60000
             SYNC_METHOD = 'RunBitLow'
             POLLING_INTERVAL = 0.1
             WAVEFORM_SOURCE = 'CHANnel1'
             NUMBER_POINTS = 640
             NUMBER_TRIGGERS = 10
             DIRECTORY = 'C:\\Users\\Public\\'
             OUTPUT_FILE = 'BOTH'
             LOAD_SAVED_FILES = True
             OPEN_CSV_FILE = True
             VERBOSE_MODE = True
        5. Run the script and watch the output in the Python console.  It will
           list the path to the CSV file it created, and the CSV file will open
           for viewing (in Excel, if Excel is installed).
        6. Now test the synchronization by disconnecting the probe from the
           signal and running the script again.  The script will wait
           indefinitely for you to reconnect the probe, after which it imports
           the waveform as in step 5.
        7. Test synchronization over multiple triggers by setting
           NUMBER_TRIGGERS = 10 and once again disconnecting the probe before
           running the script.  Touch the probe tip to the signal a few times,
           noting that you can lift the probe for any amount of time without
           any timeout errors.  When 10 waveforms have been acquired, execution
           stops and the CSV file opens.  The CSV file contains 11 columns: a
           Time column followed by 10 CHANnel1 (voltage) columns.
        8. Modify the User Inputs as needed for your application

'''

# =============================================================================
# Import Python modules
# =============================================================================

import sys
import pyvisa as visa  # PyVisa info @ http://PyVisa.readthedocs.io/en/stable/
import time
import numpy as np
import os
# from pyvisa import util

# =============================================================================
# User Inputs: I/O settings and script behavior
# =============================================================================

# VISA address ################################################################
#
# Get the VISA address (or alias) from Keysight Connection Expert
# Video: Connecting to Instruments Over LAN, USB, and GPIB in Keysight
# Connection Expert: https://youtu.be/sZz8bNHX5u4
#
#
# Example VISA address for a LAN connection:
# SCOPE_VISA_ADDRESS = 'TCPIP0::192.168.1.110::hislip0::INSTR'  # MSOX3104T
# SCOPE_VISA_ADDRESS = 'TCPIP0::192.168.1.111::hislip0::INSTR'  # MSOX3054A
# SCOPE_VISA_ADDRESS = 'TCPIP0::10.81.216.134::inst0::INSTR'  # MSOX6004A
SCOPE_VISA_ADDRESS = 'TCPIP0::192.168.153.178::inst0::INSTR'  # MSOX6004A

# Example VISA address for a USB connection:
# SCOPE_VISA_ADDRESS = 'USB0::0x2A8D::0x1770::MY56311141::0::INSTR' # MSOX3104T
# SCOPE_VISA_ADDRESS = 'USB0::0x2A8D::0x1797::CN57046145::0::INSTR' # DSOX1102G
#
# Example VISA address for a GPIB connection:
# SCOPE_VISA_ADDRESS = 'GPIB0::2::INSTR'
#
# Example VISA address for an RS-232 connection (this is SLOW!):
# SCOPE_VISA_ADDRESS = "ASRL6::INSTR"  # 54642D
#
# Example VISA address using the connection's alias from Connection Expert:
# SCOPE_VISA_ADDRESS = 'msox3104t'

# I/O timeout values ##########################################################
#
# A 5000 ms general timeout is recommended for typical use models.  Note that
# Infiniium oscilloscopes may require larger timeouts when under heavier CPU
# loads.
GENERAL_TIMEOUT = 15000  # General I/O timeout (ms)
TRIGGER_TIMEOUT = 60000  # Max time to wait for a trigger (ms) for *OPC? method

# Synchronization method ######################################################
#
# Use SYNC_METHOD to choose whether execution waits for new triggers between
# captures or simply captures whatever data is on screen without any
# synchronization.
#
# 'NONE' captures whatever is on the screen, without initiating new captures or
# waiting for triggers.  Use this if you want to import the waveform that the
# scope has already captured.
#
# ':TER?' initiates a new acquisition and waits for a trigger before each
# capture, by polling the scope repetitively with a :TER? query.  This method
# will wait indefinitely for a trigger, but generates lots of I/O traffic.
#
# '*OPC?' initiates a new acquisition and waits for a trigger before each
# capture, by sending a single :DIGitize;*OPC? query and waiting for a
# response.  This method will only wait as long as the TRIGGER_TIMEOUT value,
# but does not generate any I/O traffic while it is waiting.
#
# 'RunBitLow' initiates a new acquisition and waits for a trigger before each
# capture, by polling the scope repetitively with a :OPERegister:CONDition?
# query and waiting for bit 3 in the returned value (the Run bit) to go low,
# indicating the acquisiton has stopped.  This method will wait indefinitely
# for a trigger, but generates lots of I/O traffic.
#
#SYNC_METHOD = 'NONE'  # Imports waveform(s) without waiting for triggers
SYNC_METHOD = ':TER?'  # This polls the scope repetitively while in Run mode
#SYNC_METHOD = '*OPC?'  # This uses :DIGitize;*OPC? and waits for a 1 response
#SYNC_METHOD = 'RunBitLow'  # This polls the scope to detect when it stops

# Prevent excessive queries when polling by adding a delay in seconds:
POLLING_INTERVAL = 1./90  # (seconds)

# Define waveform source ######################################################
WAVEFORM_SOURCE = 'CHANnel1'
# WAVEFORM_SOURCE = 'WMEMory1'

# Waveform options ############################################################
# Number of waveform points to request (e.g. 1000 or 'ALL'):
# NUMBER_POINTS = 'ALL'
NUMBER_POINTS = 320

# Number of waveforms to acquire and transfer (one waveform per trigger):
NUMBER_TRIGGERS = int(sys.argv[2])

# Data save/load options ######################################################
# Define the path where the output files will be saved:
#DIRECTORY = 'C:\\Users\\Public\\'
DIRECTORY = './dataScope/'
MEASUREMENT = sys.argv[1]
# Set format for waveform data output file (CSV, BINARY, BOTH, or NONE):
OUTPUT_FILE = 'CSV'
# OUTPUT_FILE = 'BINARY'
#OUTPUT_FILE = 'BOTH'
# OUTPUT_FILE = 'NONE'

# Load data from output file(s) back into Python:
#LOAD_SAVED_FILES = True
LOAD_SAVED_FILES = False

# Set this True to open any CSV file at the end of the run:
# OPEN_CSV_FILE = True
OPEN_CSV_FILE = False

# Other options ###############################################################
# Report status details and throughput measurements:
VERBOSE_MODE = True
# VERBOSE_MODE = False

# =============================================================================
# Helper functions
# =============================================================================


def time_tag():
    time_stamp = time.localtime()
    year = time_stamp[0]
    month = time_stamp[1]
    day = time_stamp[2]
    hour = time_stamp[3]
    minute = time_stamp[4]
    second = time_stamp[5]
    time_tag = '{:d}'.format(year) + '-' + '{:0>2d}'.format(month) + '-' + \
               '{:0>2d}'.format(day) + '_' + '{:0>2d}'.format(hour) + '-' + \
               '{:0>2d}'.format(minute) + '-' + '{:0>2d}'.format(second)
    return time_tag


def binblock_raw(data_in):
    # This function interprets the header for a definite binary block
    # and returns the raw data for both definite and indefinite binary blocks
    startpos = data_in.find('#')
    if startpos < 0:
        raise IOError('No start of block found')
    lenlen = int(data_in[startpos+1:startpos+2])  # Get the data length

    # If it's a definite length binary block
    if lenlen > 0:
        # Get the length from the header
        offset = startpos+2+lenlen
        datalen = int(data_in[startpos+2:startpos+2+lenlen])
    else:
        # If an indefinite length binblock, get length from the transfer itself
        offset = startpos+2
        datalen = len(data_in)-offset-1
    # Extract the data out into a list:
    return data_in[offset:offset+datalen]


def capture_waveform():
    triggered = 0
    acq_complete = False
    if SYNC_METHOD == ':TER?':
        scope.write('*CLS;:SINGle')
        while triggered == 0:  # Poll the scope until it returns a 1
            triggered = int(scope.query(':TER?'))
    if SYNC_METHOD == '*OPC?':
        triggered = int(scope.query(':DIG;*OPC?'))
    if SYNC_METHOD == 'RunBitLow':
        scope.write('*CLS;:SINGle')
        while acq_complete is False:
     #       time.sleep(POLLING_INTERVAL)  # Prevent excessive queries
            oper = int(scope.query(':OPER:COND?'))
            acq_complete = not(bool(oper & (1 << 3)))


def get_num_points():
    global num_points_available
    global num_points
    num_points_available = int(scope.query(':WAVeform:POINts?'))
    scope.query(':WAVeform:POINts {};*OPC?'.format(NUMBER_POINTS))
    num_points = int(scope.query(':WAVeform:POINts?'))
    error_check()
    if VERBOSE_MODE:
        print(('Number of points available: {:,}'.format(num_points_available)))
        print(('Number of points requested: {:}'.format(NUMBER_POINTS)))
        print(('Number of points to import: {:}\n'.format(num_points)))


def error_check():
    # Error Check function
    global my_error
    my_error = []
    error_list = scope.query(':SYSTem:ERRor?').split(',')
    error = error_list[0]
    while int(error) != 0:
        print(('Error #: ' + error_list[0]))
        print(('Error Description: ' + error_list[1]))
        my_error.append(error_list[0])
        my_error.append(error_list[1])
        error_list = scope.query(':SYSTem:ERRor?').split(',')
        error = error_list[0]
        my_error = list(my_error)
    if my_error != []:
        safe_exit_custom_message('Run aborted due to errors.\n')


def safe_exit_custom_message(message):
    # Safe exit function
    scope.clear()
    scope.query(':STOP;*OPC?')
    scope.clear()
    scope.close()
    sys.exit(message)


# =============================================================================
# Connect and initialize scope
# =============================================================================

# Define VISA Resource Manager
# This directory will need to be changed if VISA was installed somewhere else:
#rm = visa.ResourceManager('C:\\Windows\\System32\\visa32.dll') 
rm = visa.ResourceManager()

# Open connection to the scope by its VISA address
try:
    print(('\nConnecting to: {}'.format(SCOPE_VISA_ADDRESS)))
    scope = rm.open_resource(SCOPE_VISA_ADDRESS)
except Exception:
    print(('Unable to connect to oscilloscope at {}.  Aborting.\n'
          .format(SCOPE_VISA_ADDRESS)))
    sys.exit()

# Set global timeout
scope.timeout = GENERAL_TIMEOUT

# Clear the remote interface:
scope.clear()
error_check()

idn = str(scope.query('*IDN?'))
print(('Connected to: {}'.format(idn)))
idn = idn.split(',')
model = idn[1]

# Determine Run/Stop state, to be restored later:
oper = int(scope.query(':OPER:COND?'))
run_state = bool(oper & (1 << 3))  # Use the Run bit

# =============================================================================
# Misc tasks
# =============================================================================

folder = DIRECTORY
scale_waveform = True
wav_format = 'WORD'
bin_format = 'h'
number_segments = 1

if VERBOSE_MODE:
    print(('Using SYNC_METHOD = {}'.format(SYNC_METHOD)))

date_time = time_tag()
error_check()

# Default to one data import if user constant isn't properly defined:
try:
    int(NUMBER_TRIGGERS)
except NameError:
    num_triggers = 1
else:
    num_triggers = int(NUMBER_TRIGGERS)

# A bit of format polishing for those with OCD:
if num_triggers > 1:
    s_if_plural = 's'
    s_if_plural_not = ''
else:
    s_if_plural = ''
    s_if_plural_not = 's'

# =============================================================================
# Prepare for waveform data transfer
# =============================================================================

acq_mode = str(scope.query(':ACQuire:MODE?').strip('\n'))
if acq_mode == 'SEGM':
    acq_mode = 'segmented '
    scope.write(':WAVeform:SEGMented:ALL ON')
    number_segments = int(scope.query(':WAVeform:SEGMented:COUNt?'))
    segment_time_tags = scope.query(':WAVeform:SEGMented:XLISt? TTAG')
    segment_time_tags = binblock_raw(segment_time_tags).split(',')
    for i, ttag in enumerate(segment_time_tags):
        segment_time_tags[i] = float(ttag)
    if VERBOSE_MODE:
        print(('Number of segments to be imported: {}'.format(number_segments)))
else:
    acq_mode = ''
    number_segments = 1
scope.query(':WAVeform:SOURce {};*OPC?'.format(WAVEFORM_SOURCE))
scope.write(':WAVeform:FORMat {}'.format(wav_format))
if wav_format != 'ASCii':
    scope.write(':WAVeform:BYTeorder LSBFirst')
    scope.write(':WAVeform:UNSigned 0')
scope.write(':WAVeform:POINts:MODE RAW')
scope.write(':WAVeform:POINts MAX')

# =============================================================================
# Capture and import waveform(s)
# =============================================================================

if VERBOSE_MODE:
    print(('Capturing {:,} {}waveform{}...\n'
          .format(NUMBER_TRIGGERS, acq_mode, s_if_plural)))

start_time = time.time()  # Time acquiring waveforms and importing the data
scope.timeout = TRIGGER_TIMEOUT

get_num_points()

for i in range(num_triggers):
    if SYNC_METHOD != 'NONE':
        capture_waveform()
    if i == 0:
        num_points_total = num_points*number_segments
        waveforms = np.zeros((num_points_total*NUMBER_TRIGGERS))
    waveforms[i*num_points:(i*num_points)+num_points] = np.array(scope.query_binary_values(
            ':WAVeform:DATA?', '{}'.format(bin_format), False))

scope.timeout = GENERAL_TIMEOUT
acquisition_time = time.time() - start_time
acquisition_rate = NUMBER_TRIGGERS/acquisition_time

if VERBOSE_MODE:
    print(('Done capturing and importing {:,} '.format(NUMBER_TRIGGERS)
          + '{:,}-point {}waveform{}.\n'
          .format(num_points, acq_mode, s_if_plural)
          + 'This took {:.3f} seconds '.format(acquisition_time)
          + '({:.1f} waveforms/s).'.format(acquisition_rate) + '\n'))

# Scale the waveform:
if VERBOSE_MODE:
    print(('Scaling {:,} {}waveform{}...'
          .format(NUMBER_TRIGGERS, acq_mode, s_if_plural)))
    start_time = time.time()  # Time how long it takes to scale the data

preamble = scope.query(':WAVeform:PREamble?').split(',')
wfm_type = int(preamble[1])
X_INCrement = float(preamble[4])
X_ORigin = float(preamble[5])
X_REFerence = float(preamble[6])
Y_INCrement = float(preamble[7])
Y_ORigin = float(preamble[8])
Y_REFerence = float(preamble[9])

# Calculate time axis for one waveform (or a single segment):
time_axis = ((np.linspace(0, num_points-1, num_points) - X_REFerence)
             * X_INCrement) + X_ORigin
print(type(time_axis))
# Calculate time axis for all segments:  TODO: Fix this!
x_axis = []
if acq_mode == 'segmented ':
    for i in range(number_segments):
        x = time_axis + segment_time_tags[i]
        x_axis = np.stack(x_axis, x)
waveforms = ((waveforms - Y_REFerence) * Y_INCrement) + Y_ORigin

scaling_time = time.time() - start_time
scaling_rate = NUMBER_TRIGGERS/scaling_time

if VERBOSE_MODE:
    print(('Done scaling {:,} '.format(NUMBER_TRIGGERS)
          + '{:,}-point {}waveform{}.\n'
          .format(num_points, acq_mode, s_if_plural)
          + 'This took {:.3f} seconds '.format(scaling_time)
          + '({:.1f} waveforms/s).\n'.format(scaling_rate)))

# =============================================================================
# Create file name with unique date/time tag
# =============================================================================

time_stamp = time.localtime()
year = time_stamp[0]
month = time_stamp[1]
day = time_stamp[2]
hour = time_stamp[3]
minute = time_stamp[4]
second = time_stamp[5]
date_stamp = '{:d}'.format(year) + '-' + '{:0>2d}'.format(month) + '-' + \
            '{:0>2d}'.format(day) + '_' + '{:0>2d}'.format(hour) + '-' + \
            '{:0>2d}'.format(minute) + '-' + '{:0>2d}'.format(second)

filename = DIRECTORY + MEASUREMENT # date_stamp

# =============================================================================
# Save waveform data to CSV file and load it back into Python
# =============================================================================

if OUTPUT_FILE == 'CSV' or OUTPUT_FILE == 'BOTH':  # Save CSV file
    if VERBOSE_MODE:
        print(('Saving {:,} waveform{} in CSV format...'.format(
                NUMBER_TRIGGERS, s_if_plural)))
    start_time = time.time()  # Time saving waveform data to a CSV file
    header = 'Time (s),{} (V)\n'.format(WAVEFORM_SOURCE)
    csv_file = filename + '.csv'
    with open(csv_file, 'w') as filehandle:
        #filehandle.write(header)
        np.savetxt(filehandle, waveforms,
                   delimiter=',')
    time_csv_file = filename +"_time.csv"
    with open(time_csv_file,'w') as filehandle_time:
        np.savetxt(filehandle_time,time_axis, delimiter=',')

    csv_saving_time = time.time() - start_time
    csv_saving_rate = NUMBER_TRIGGERS/csv_saving_time
    print(('Done saving {:,} '.format(NUMBER_TRIGGERS)
          + '{:,}-point waveform{} to file:\n'.format(
                  num_points, s_if_plural) + '    ' + csv_file))
    if VERBOSE_MODE:
        print(('This took {:.3f} seconds '.format(csv_saving_time)
              + '({:.1f} waveforms/s).\n'.format(csv_saving_rate)))
    del start_time
    if LOAD_SAVED_FILES:
        if VERBOSE_MODE:
            print('Loading CSV waveform data back into Python...')
        start_time = time.time()
        with open(csv_file, 'r') as filehandle:  # r means open for reading
            recalled_CSV = np.loadtxt(filehandle, delimiter=',', skiprows=1)
        csv_loading_time = time.time() - start_time
        csv_loading_rate = NUMBER_TRIGGERS/csv_loading_time
        del filehandle, header
        if VERBOSE_MODE:
            print(('Done loading waveform data into NumPy array: recalled_CSV\n'
                  + 'This took {:.3f} seconds '.format(csv_loading_time)
                  + '({:.1f} waveforms/s).\n'.format(csv_loading_rate)))
        del start_time
    if OPEN_CSV_FILE:
        print('CSV file opened for viewing.\n')
        os.startfile(csv_file)

# =============================================================================
# Save waveform data to numpy file and load it back into Python
# =============================================================================

if OUTPUT_FILE == 'BINARY' or OUTPUT_FILE == 'BOTH':  # Save NPY file
    if VERBOSE_MODE:
        print(('Saving {:,} waveform{} in binary (NumPy) format...'
              .format(NUMBER_TRIGGERS, s_if_plural)))
    start_time = time.time()  # Time saving waveform data to a numpy file
    header = 'Time (s),{} (V)\n'.format(WAVEFORM_SOURCE)
    npy_file = filename + '.npy'
    with open(npy_file, 'wb') as filehandle:
        np.save(filehandle, np.insert(waveforms, 0, time_axis, axis=1))
        npy_saving_time = time.time() - start_time
        npy_saving_rate = NUMBER_TRIGGERS/npy_saving_time
    print(('Done saving {:,} '.format(NUMBER_TRIGGERS)
          + '{:,}-point waveform{} to file:\n'.format(
                  num_points, s_if_plural) + '    ' + npy_file))
    if VERBOSE_MODE:
        print(('This took {:.3f} seconds '.format(npy_saving_time)
              + '({:.1f} waveforms/s).\n'.format(npy_saving_rate)))
    if LOAD_SAVED_FILES:
        if VERBOSE_MODE:
            print('Loading NumPy waveform data back into Python...')
        start_time = time.time()  # Time loading waveform data from NPY file
        with open(npy_file, 'rb') as filehandle:  # rb means read binary
            recalled_NPY = np.load(filehandle)
        npy_loading_time = time.time() - start_time + 0.001  # Avoid div by 0
        npy_loading_rate = NUMBER_TRIGGERS/npy_loading_time
        del filehandle, npy_file, header
        if VERBOSE_MODE:
            print(('Done loading waveform data into NumPy array: recalled_NPY\n'
                  + 'This took {:.3f} seconds '.format(npy_loading_time)
                  + '({:.1f} waveforms/s).\n'.format(npy_loading_rate)))
        del start_time

# =============================================================================
# Restore the original Run/Stop state
# =============================================================================

if run_state:
    scope.write(':RUN')
else:
    scope.write(':STOP')

# =============================================================================
# Close connection to scope
# =============================================================================

error_check()
scope.clear()
scope.close()

print('Done.')
