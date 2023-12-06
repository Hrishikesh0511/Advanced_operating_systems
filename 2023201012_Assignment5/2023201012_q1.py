import time
from pyspark.sql import SparkSession
from pyspark.sql.functions import desc
start_time=time.time()
# Initialize a Spark session
spark = SparkSession.builder.appName("HousePricing").getOrCreate()

df=spark.read.csv('House_Pricing.csv', header=True, inferSchema=True)

selected_countries=['GREATER LONDON', 'CLEVELAND', 'ESSEX']
country_filter=df.filter(df['Country'].isin(selected_countries))

sorted_data=country_filter.sort(desc('Price'))

first_highest=sorted_data.select('Price').collect()[0][0]
second_highest=sorted_data.select('Price').collect()[1][0]

i=1
while first_highest==second_highest:
   second_highest=sorted_data.select('Price').collect()[i][0]
   i+=1

country=sorted_data.filter(sorted_data['Price']==second_highest).select('Country').collect()[0][0]

end_time=time.time()

print(f"Second highest transacted value is {second_highest} which is transacted in {country}.")
elapsed_time=end_time-start_time
print(f"Time Elapsed : {elapsed_time} seconds.")
