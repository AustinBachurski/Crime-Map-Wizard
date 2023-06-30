Script runs three geoprocessing tools: Table to Point XY, Delete Features, and Append Features.

Credential's are read from local environment variables, script modification is not necessary.

If you receive a "Tool is not licensed" error, simply open ArcGIS Pro and sign in, then run the tool again.





###########################################################################################################
############################################# DEBUGGING ###################################################
###########################################################################################################


For assistance in debugging, you can change the debug variable on line 24 to True, this will result in 
additional console outputs and the console window will remain open until you close it.

- Don't forget to set debug to False when you're done.