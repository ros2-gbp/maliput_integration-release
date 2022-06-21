\page maliput_to_string_app maliput_to_string application

# Inspecting RoadGeometry

`maliput_to_string` application builds a maliput::api::RoadGeometry and lists its entities. The backends that can be selected to build the RoadGeometry are `maliput_malidrive`, `maliput_multilane` and `maliput_dragway`.

The application possesses flags to modify the output serialization and include information related to entities as maliput::api::Lane IDs, maliput::api::Segment IDs, maliput::api::Junction IDs, maliput::api::RoadGeometry ID.
```
--include_type_labels, --include_road_geometry_id, --include_junction_ids,
--include_segment_ids, --include_lane_ids, --include_lane_details.
```
All `--include_*` flags can be combined to print all the needed information.

Depending on the maliput backend that is selected different flags related to the RoadGeometry building process will be active.
 - maliput_malidrive backend: See MALIDRIVE_PROPERTIES_FLAGS().
 - maliput_multilane backend: See MULTILANE_PROPERTIES_FLAGS().
 - maliput_dragway backend: See DRAGWAY_PROPERTIES_FLAGS().


### Using maliput_malidrive backend

```bash
$ maliput_to_string --maliput_backend=malidrive --xodr_file_path=TShapeRoad.xodr --linear_tolerance=0.05 --include_lane_ids
```

Output:
```
[INFO] Loading road network using malidrive backend implementation...
[INFO] RoadNetwork loaded successfully.
  0_0_-4
  0_0_-3
  0_0_-2
  0_0_-1
  0_0_1
  0_0_2
  0_0_3
  0_0_4
  1_0_-4
  1_0_-3
  1_0_-2
  1_0_-1
  1_0_1
  1_0_2
  1_0_3
  1_0_4
  2_0_-4
  2_0_-3
  2_0_-2
  2_0_-1
  2_0_1
  2_0_2
  2_0_3
  2_0_4
  4_0_1
  5_0_-1
  6_0_-1
  7_0_-1
  8_0_-1
  9_0_-1
```
All the maliput::api::Lane IDs are printed as `--include_lane_ids` flag was added.

_Note: To increase verbosity the log level could be adjusted to `TRACE` by adding the `--log_level=trace` flag._

### Using maliput_multilane backend

Similarly, `maliput_multilane` can be used.

```bash
$ maliput_to_string --maliput_backend=multilane --yaml_file=2x2_intersection.yaml --include_lane_ids
```

Output:
```
[INFO] Loading road network using multilane backend implementation...
[INFO] RoadNetwork loaded successfully.
l:ew_intersection_segment_0
l:ew_intersection_segment_1
l:ns_intersection_segment_0
l:ns_intersection_segment_1
l:east_right_turn_segment_0
l:west_right_turn_segment_0
l:north_right_turn_segment_0
l:south_right_turn_segment_0
l:east_left_turn_segment_0
l:west_left_turn_segment_0
l:north_left_turn_segment_0
l:south_left_turn_segment_0
l:e_segment_0
l:e_segment_1
l:n_segment_0
l:n_segment_1
l:s_segment_0
l:s_segment_1
l:w_segment_0
l:w_segment_1

```

### Using maliput_dragway backend

Similarly, `maliput_dragway` can be used.

```bash
$ maliput_to_string --maliput_backend=dragway --num_lanes=3 --length=5 --lane_width=3 --shoulder_width=0.5 --maximum_height=4 --include_lane_ids --include_lane_details
```
Notice that `--include_lane_details` was added.

Output:
```
[INFO] Loading road network using dragway backend implementation...
[INFO] RoadNetwork loaded successfully.
Dragway_Lane_0
  length: 5
  geo positions:
    s_min: (x = 0, y = -3, z = 0)
    s_max: (x = 5, y = -3, z = 0)
  to left: Dragway_Lane_1
  to right:
Dragway_Lane_1
  length: 5
  geo positions:
    s_min: (x = 0, y = 0, z = 0)
    s_max: (x = 5, y = 0, z = 0)
  to left: Dragway_Lane_2
  to right: Dragway_Lane_0
Dragway_Lane_2
  length: 5
  geo positions:
    s_min: (x = 0, y = 3, z = 0)
    s_max: (x = 5, y = 3, z = 0)
  to left:
  to right: Dragway_Lane_1
```

## More available options

As mentioned before, `maliput_to_string` application has several arguments that can be used. All of them can be accessed by running `maliput_to_string --help`.

Use `--check_invariants` to enable maliput invariants verification. See maliput::api::RoadGeometry::CheckInvariants() .

Use `--log_level` to set the log output See possible values at maliput::common::logger::level. By default set to `unchanged`.

## Other maliput_to_string implementations

There are two more variants of `maliput_to_string` app that you can find within `maliput_integration` package.

They are reduced versions of `maliput_to_string` however they were meant to exemplify how to use maliput and its backend using the plugin architecture. (See maliput::plugin::MaliputPlugin())


### maliput_to_string_with_plugin:

Based on the plugin architecture that maliput provides for loading the RoadNetwork the application lets you to indicate which plugin (backend) you want to use.


For example using `--plugin_name=maliput_malidrive`
```bash
maliput_to_string_with_plugin --plugin_name=maliput_malidrive --include_lane_ids --opendrive_file=TShapeRoad.xodr
```

Output:
```
[INFO] Creating MaliputPluginManager instance...
[INFO] Plugin Id: maliput_multilane was correctly loaded.
[INFO] Plugin Id: maliput_malidrive was correctly loaded.
[INFO] Plugin Id: maliput_dragway was correctly loaded.
[INFO] Number of plugins loaded: 3
[INFO] Plugins loading is completed.
[INFO] maliput_malidrive plugin has been found.
[INFO] Plugin id: maliput_malidrive
[INFO] Plugin type: RoadNetworkLoader
[INFO] RoadGeometry loaded successfully after iteration [0] using:
	|__ linear_tolerance = 0.05
	|__ angular_tolerance = 0.001
	|__ scale_length = 1
[INFO] RoadNetwork loaded successfully.
0_0_-1
0_0_1
1_0_-1
1_0_1
2_0_-1
2_0_1
4_0_1
5_0_-1
6_0_-1
7_0_-1
8_0_-1
9_0_-1
```

### maliput_to_string_with_plugin.py

Similarly to `maliput_to_string_with_plugin` in which the plugin architecture is in between however in this case, python bindings are used.

```bash
maliput_to_string_with_plugin.py -plugin_name maliput_malidrive -opendrive_file=TShapeRoad.xodr
```

Output:
```bash
[INFO] Plugin Id: maliput_multilane was correctly loaded.
[INFO] Plugin Id: maliput_malidrive was correctly loaded.
[INFO] Plugin Id: maliput_dragway was correctly loaded.
[INFO] Number of plugins loaded: 3
[INFO] RoadGeometry loaded successfully after iteration [0] using:
	|__ linear_tolerance = 0.05
	|__ angular_tolerance = 0.01
	|__ scale_length = 1

Road Geometry ID:  maliput
Number of junctions:
	Junction  0
		Number of segments  1
		Segment  0
			Number of lanes:  2
			Lane id:  0_0_-1
				length:  45.99999999999975
			Lane id:  0_0_1
				length:  45.99999999999975
	Junction  1
		Number of segments  1
		Segment  0
			Number of lanes:  2
			Lane id:  1_0_-1
				length:  45.99999999999975
			Lane id:  1_0_1
				length:  45.99999999999975
	Junction  2
		Number of segments  1
		Segment  0
			Number of lanes:  2
			Lane id:  2_0_-1
				length:  45.99999999999975
			Lane id:  2_0_1
				length:  45.99999999999975
	Junction  3
		Number of segments  6
		Segment  0
			Number of lanes:  1
			Lane id:  4_0_1
				length:  7.999999999999803
		Segment  1
			Number of lanes:  1
			Lane id:  5_0_-1
				length:  7.999999999999803
		Segment  2
			Number of lanes:  1
			Lane id:  6_0_-1
				length:  9.061381870907974
		Segment  3
			Number of lanes:  1
			Lane id:  7_0_-1
				length:  3.563590834716626
		Segment  4
			Number of lanes:  1
			Lane id:  8_0_-1
				length:  9.061381870907978
		Segment  5
			Number of lanes:  1
			Lane id:  9_0_-1
				length:  3.563590834715477
```