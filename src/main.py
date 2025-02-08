import geopandas as gpd

s57_file = "CN201301.000"


# 1 degree in lon/lat is approx 111km
def s57_to_vec_list(file_name, buffer_size=0.00001):
    try:
        gdf = gpd.read_file(file_name, layer="LNDARE")
    except:
        print("Read s57 LNDARE failed, check file name or if layer exists.")
        exit()

    expanded_geo_list = [geo_obj.buffer(buffer_size) for geo_obj in gdf.geometry]
    return expanded_geo_list

if __name__ == "__main__":
    geo_list = s57_to_vec_list(s57_file)
    # print the first three objects in this demo
    print(geo_list[:3])