import time
from pyspark.sql import SparkSession
from pyspark.sql.functions import count
start_time=time.time()

spark = SparkSession.builder.appName("TransactionAnalysis").getOrCreate()

df = spark.read.csv("House_Pricing.csv", header=True, inferSchema=True)

transaction_count_df = df.groupBy("Country").agg(count("*").alias("Count"))

transaction_count_df.show()

transaction_count_df.write.csv("2023201012_q3", header=True)
end_time=time.time()
elapsed_time=end_time-start_time
print(f"Elapsed time :{elapsed_time}seconds")
