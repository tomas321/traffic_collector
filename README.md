## Traffcol

C++ network traffic sensor utilizing Netmap capture mechanism and stores data in ELK Stack for further processing, analyzing and visualizing

### License

None

### Authors

Tomas Bellus

### USAGE
#### Start Traffcol

1. Refere to the `INSTALL.txt` file for compilation and installation.
2. Modify `<REPO_ROOT>/resources/config.yml` accordingly.
3. Run any of the built targets from <REPO_ROOT>.

NOTE: If netmap-based libpcap is not your installed libpcap library:
For running 'traffcol' main executable you must have super-user priveleges
and prepend the execution with `LD_LIBRARY_PATH` variable setting the path to
netmap-based libpcap directory containing the library file.

##### Configuration validation
1. Locate to <REPO_ROOT>
2. Run `./build/traffcol-validate`

##### Print configuration file
1. Locate to <REPO_ROOT>.
2. Run `./build/traffcol-print-config`.

##### Start sensor for packet capture
1. Locate to <REPO_ROOT>.
2. Run `sudo ./build/traffcol`.

#### Start Kibana

After successfully compiling the sensor, installing and configuring the ELK
stack with initialized index template and loaded visualizations and dashboards,
you are ready to use the Kibana for monitoring your network (if sensor is
running and data is flowing).

1. Open Kibana in your favorite browser by going to <host>:<port> address (if
   the port was not change during installation its 5601)
2. Kibana offers to try their sample data for experimenting. Choose the other
   option for exploring your own data. (applies to first open)
3. Locate to the `Dashboard` from the side bar and select the dashboard from
   list.
4. Possible errors will pop-up on the top of the page if no data is yet
   present.
5. Monitor your network traffic by manipulating filters and time ranges. 
