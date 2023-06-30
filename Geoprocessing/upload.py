"""
Performs Table to Point XY, Delete Features, and Append Features geoprocessing tools.

- The DEBUG Flag on line 18 can be set to True to aid in debugging.

Return Codes are as follows:
    - 0: Success.
    - 1: Sign in to ArcGIS Online Failed.
    - 2: Crime Data.csv could not be located - encountered during Table to Point.
    - 3: URL to feature layer is invalid - encountered during Delete Features.
    - 4: URL to feature layer is invalid - encountered during Append Features.
"""
import os
import time
import threading


DEBUG = False  # Set to True for additional console outputs, should be False during normal operation.

#######################################################################################################################
################################### NO MODIFICATIONS ARE REQUIRED BELOW THIS LINE! ####################################
#######################################################################################################################

spin = None  # Used for the "work in progress" indicator.


def get_csv_file():
    os.chdir('..')
    return os.path.join(os.getcwd(), "Data\\Crime Data.csv")


def start_spinning(message):
    global spin
    spin = True
    spinner = ['/', '-', '\\', '|']
    print(f"\r{message}", end='', flush=True)
    time.sleep(.5)
    while spin:
        for spinny in spinner:
            print(f"\r{message}{spinny}", end='', flush=True)
            time.sleep(0.25)


def stop_spinning():
    global spin
    spin = False
    time.sleep(2)


def upload_data():
    global DEBUG
    print("Preparing to perform geoprocessing tasks, one moment please...", end='', flush=True)
    feature_layer = os.getenv("CRIMEDATA_LAYER")
    import arcpy  # ArcPy takes some time to import, do it here so the user doesn't sit at a blank screen.
    arcpy.env.overwriteOutput = True
    os.system("cls")

    signing_in_message = "Signing in to ArcGIS Online..."
    threading.Thread(target=lambda: start_spinning(signing_in_message)).start()
    try:  # Signing in to ArcGIS Online.
        arcpy.SignInToPortal("https://www.arcgis.com/",
                             os.getenv("AGOL_USERNAME"),
                             os.getenv("AGOL_PASSWORD"))
    except ValueError as error:
        stop_spinning()
        if DEBUG:
            input(f"\n\nERROR: Failed to sign in to ArcGIS Online.\n"
                  f"\nError received: {error}\n\n"
                  f"Please inform Austin Bachurski of the error, his contact info\n"
                  f"can be found by clicking 'Help' in the main window.\n\n"
                  f"Press 'enter' to close this window.")
        sys.exit(1)
    else:
        stop_spinning()
        print(f"\r{signing_in_message}success.", flush=True)

    table_to_point_message = "Converting CSV information to GIS points..."
    threading.Thread(target=lambda: start_spinning(table_to_point_message)).start()
    try:  # Running TableToPoint geoprocessing tool.
        arcpy.management.XYTableToPoint(in_table=get_csv_file(),
                                        out_feature_class="memory/CrimeData",  # Stores output of geoprocessing tool in memory.
                                        x_field="Longitude",
                                        y_field="Latitude",
                                        coordinate_system=arcpy.SpatialReference(4326))
    except FileNotFoundError as error:
        stop_spinning()
        print('\n', flush=True)
        if DEBUG:
            input(f"\n\nERROR: OS could not find the 'Crime Data.csv' file at.\n"
                  f"{get_csv_file()}\n"
                  f"\nError received: {error}\n\n"
                  f"Please inform Austin Bachurski of the error, his contact info\n"
                  f"can be found by clicking 'Help' in the main window.\n\n"
                  f"Press 'enter' to close this window.")
        sys.exit(2)
    except arcpy.ExecuteError as error:
        stop_spinning()
        if DEBUG:
            input(f"\n\nERROR: OS could not find the 'Crime Data.csv' file at.\n"
                  f"{get_csv_file()}\n"
                  f"\nError received: {error}\n\n"
                  f"Please inform Austin Bachurski of the error, his contact info\n"
                  f"can be found by clicking 'Help' in the main window.\n\n"
                  f"Press 'enter' to close this window.")
        sys.exit(2)
    else:
        stop_spinning()
        print(f"\r{table_to_point_message}done.", flush=True)

    delete_features_message = "Removing old data from the web map..."
    threading.Thread(target=lambda: start_spinning(delete_features_message)).start()
    try:  # Running Delete Features geoprocessing tool.
        arcpy.management.DeleteFeatures(feature_layer)
    except arcpy.ExecuteError as error:
        stop_spinning()
        if DEBUG:
            input(f"\n\nERROR: The web layer was not found at:\n"
                  f"{feature_layer}\n"
                  f"\nError received: {error}\n\n"
                  f"Please inform Austin Bachurski of the error, his contact info\n"
                  f"can be found by clicking 'Help' in the main window.\n\n"
                  f"Press 'enter' to close this window.")
        sys.exit(3)
    else:
        stop_spinning()
        print(f"\r{delete_features_message}done.", flush=True)

    append_features_message = "Uploading new data to the web map..."
    threading.Thread(target=lambda: start_spinning(append_features_message)).start()
    try:  # Running Append Features geoprocessing tool.
        arcpy.management.Append(inputs="memory/CrimeData",
                                target=feature_layer,
                                schema_type="NO_TEST")
    except arcpy.ExecuteError as error:
        stop_spinning()
        if DEBUG:
            input(f"\n\nERROR: The web layer was not found at:\n"
                  f"{feature_layer}\n"
                  f"\nError received: {error}\n\n"
                  f"Please inform Austin Bachurski of the error, his contact info\n"
                  f"can be found by clicking 'Help' in the main window.\n\n"
                  f"Press 'enter' to close this window.")
        sys.exit(4)
    else:
        stop_spinning()
        print(f"\r{append_features_message}done.", flush=True)

    print(f"\nGeoprocessing complete.")
    time.sleep(2)
    sys.exit(0)


if __name__ == "__main__":
    upload_data()
