import pandas as pd
import numpy as np

df = pd.read_csv("data/ch2_scores_em.csv",
                 index_col="student number")

scores = np.array(df["english"])[:10]
scores_df = pd.DataFrame({"score": scores},
                         index=pd.Index(['A','B','C','D','E','F','G','H','I','J'],
                                        name="student"))

# 평균을 구하는 세 가지 방법
# print(sum(scores)/len(scores))
# print(np.mean(scores))
# print(scores_df.mean())

# 중앙값을 구하는 방법
# print(np.median(scores))
# print(scores_df.median())

# 최빈값을 구하는 방법
# series = pd.Series([1,1,1,2,2,3])
# print(series.mode())

# series = pd.Series([1,2,3,4,5])
# print(series.mode())

# 편차를 구하는 방법
mean = np.mean(scores)
deviation = scores - mean
# print(deviation)

# 분산을 구하는 세 가지 방법
# print(np.mean(deviation**2))
# print(np.var(scores))
# print(scores_df.var(ddof=0))

# 불편 분산
# print(scores_df.var())

# 편차와 분산을 dataframe에 추가
summary_df = scores_df.copy()
summary_df["deviation"] = summary_df - summary_df.mean()
summary_df["square of deviation"] = np.square(summary_df['deviation'])

print(summary_df)

# 데이터 지표 정리
print(scores_df.describe())