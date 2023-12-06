import time
from pyspark.sql import SparkSession
from pyspark.sql.functions import *

start_time=time.time()
spark=SparkSession.builder.appName("TransactionAnalysis").getOrCreate()

df=spark.read.csv("House_Pricing.csv", header=True, inferSchema=True)

transaction_count_df=df.groupBy("Country").count()
#returns a dataframe consists of a Country,count columns
sorted_df = transaction_count_df.sort(desc("count"))
max_count=sorted_df.collect()[0]["count"]
second_highest_country=sorted_df.where((col("count")!=max_count)).collect()[0]["Country"]

end_time=time.time()
# Print the result
print(f"Country with the second most transactions is '{second_highest_country}'.")
elapsed_time=end_time-start_time
print(f"Elapsed_time : {elapsed_time} seconds")

