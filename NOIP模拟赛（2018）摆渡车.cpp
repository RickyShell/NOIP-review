#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;
typedef long long ll;
ll reach[501];
ll sum[4000001],peo[4000001];
ll dp[4000001];
int main(){
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	ll limit=0;
	for(ll i=1;i<=n;i++){
		cin>>reach[i];
		peo[reach[i]]++;
		sum[reach[i]]+=reach[i];
		limit=max(limit,reach[i]);
	}
	for(ll i=1;i<=limit+m;i++){
		peo[i]+=peo[i-1];
		sum[i]+=sum[i-1];
	}
	for(ll i=1;i<=limit+m;i++){
		if(i>=m&&peo[i-m]==peo[i]){
			dp[i]=dp[i-m];
			continue;
		}
		dp[i]=peo[i]*i-sum[i];
		ll t;
		t=max(i-2*m+1,0ll);
		for(ll j=t;j<=i-m;j++){
			dp[i]=min(dp[i],dp[j]+(peo[i]-peo[j])*i-(sum[i]-sum[j]));
		}
	}
	ll ans=999121212;
	for(ll i=limit;i<=limit+m;i++){
		ans=min(dp[i],ans);
	}
	cout<<ans;
	return 0;
}

-----------------------------------------------------------------------------------------------------------------------------
摆渡车的题解我已经写过一遍了，在这里，这次主要从斜率优化的角度讲一下摆渡车，并总结一下斜率优化会出现的一些奇奇怪怪的错误。


摆渡车#
Description#
有 n 名同学要乘坐摆渡车从人大附中前往人民大学，第 i 位同学在第 titi分钟去 等车。只有一辆摆渡车在工作，但摆渡车容量可以视为无限大。摆渡车从人大附中出发、 把车上的同学送到人民大学、再回到人大附中（去接其他同学），这样往返一趟总共花费m分钟（同学上下车时间忽略不计）。摆渡车要将所有同学都送到人民大学。

凯凯很好奇，如果他能任意安排摆渡车出发的时间，那么这些同学的等车时间之和最小为多少呢？

注意：摆渡车回到人大附中后可以即刻出发。

Input Format#
第一行包含两个正整数 n,m，以一个空格分开，分别代表等车人数和摆渡车往返 一趟的时间。

第二行包含 n 个正整数，相邻两数之间以一个空格分隔，第 i 个非负整数 ti代 表第 i 个同学到达车站的时刻。

Output Format#
输出一行，一个整数，表示所有同学等车时间之和的最小值（单位：分钟）。

Sample Input#
Copy
5 5 
11 13 1 5 5
Sample Output#
Copy
4
解析#
如果考虑斜率优化的思路的话，我们可以这样设置状态：fifi代表到了时间点ii，所有同学等待时间的最小和。那么就可以这样写状态转移方程：

fi=minj≤i−m{fj+∑j<tk≤i(i−tk)}
fi=minj≤i−m{fj+∑j<tk≤i(i−tk)}

然后可以用前缀和把求和式拆一下，设cnticnti代表到时间点ii位置已经到达过车站的学生个数，sumisumi代表已经到达过车站的学生的到达时间总和，则原方程可以写为：

fi=minj≤i−m{fj+(cnti−cntj)∗i−(sumi−sumj)}
fi=minj≤i−m{fj+(cnti−cntj)∗i−(sumi−sumj)}

好了，看到有i,ji,j乘积项就可以考虑斜率优化了，我们再整理一下式子：

fi=minj≤i−m{fj+sumj−cntj∗i}+cnti∗i−sumi
fi=minj≤i−m{fj+sumj−cntj∗i}+cnti∗i−sumi

假设找到了最优的决策点jj，那么就有：

fi=fj+sumj−cntj∗i+cnti∗i−sumifj+sumj=cntj∗i+fi−cnti∗i+sumi
fi=fj+sumj−cntj∗i+cnti∗i−sumifj+sumj=cntj∗i+fi−cnti∗i+sumi

把fj+sumjfj+sumj看做yy，把cntjcntj看做xx，把ii看作kk，把fi−cnti∗i+sumifi−cnti∗i+sumi看做bb，这就是直线方程了，然后直接维护下凸壳，利用决策单调性转移就可以了。

但其实这道题还没那么简单，其实还有如下几个问题：

1.1. 变量jj有取值范围，需满足j≤i−mj≤i−m。

2.2. 计算斜率可能出现分母为00。

3.3. ff数组缺少部分初值。

对于第一个问题，我们采用分层推入队列的方法：在维护单调队列时，每当完成了对fifi的转移，我们应尝试把i−m+1i−m+1这一个决策点推入队列，这样就能保证满足决策变量jj的取值范围合法，其他题也是一样的。

对于第二个问题，计算斜率时分母为00就对应了坐标系内两个点坐标横坐标相同，但是纵坐标不同的情况。相应地，我们将这种情况视为两点之间的斜率为+∞+∞或−∞−∞(按照两点位置判断)处理即可，这样处理既简单，又对正确性没有影响。

第三个问题就对应了第一个问题，对于前mm的点，显然我们是没有初值的，也无法用单调队列转移，所以我们必须要暴力对前mm个状态进行更新，才能进行斜率优化，这个就根据dpdp式来更新就可以了。

Code:Code:

Copy
#include <bits/stdc++.h>
using namespace std;
const long long N=1e6+20,M=1e6+20,T=9e6+20,INF=1e18;
long long n,m,maxT,c[T],s[T],f[T];
long long q[T],head,tail,ans=INF;
inline void input(void)
{
    scanf("%lld%lld",&n,&m);
    for (int i=1;i<=n;i++)
    {
        long long t;scanf("%lld",&t);
        maxT = max(t,maxT);
        s[t] += t; c[t] ++; 
    }
}
inline void init(void)
{
    for (int i=1;i<=maxT+m;i++)
        c[i] += c[i-1] , s[i] += s[i-1];
}
inline double slope(int x,int y)
{
    if ( c[x] == c[y] ) return f[y]+s[y] > f[x]+s[x] ? INF * 1.0 : INF * -1.0;
    return ( 1.0 * (f[y]+s[y]) - 1.0 * (f[x]+s[x]) ) / ( 1.0 * (c[y]) - 1.0 * (c[x]) );
}
//f[i] = min{f[j]+(c[i]-c[j])*i-(s[i]-s[j])}
inline void dp(void)
{
    head = tail = 1; q[tail] = 0;
    for (int i=1;i<m;i++)
        f[i] = c[i] * i - s[i];
    for (int i=m;i<=maxT+m;i++)
    {
        while ( head<tail && slope(q[head],q[head+1]) <= 1.0 * i ) head++;
        f[i] = f[q[head]] + (c[i]-c[q[head]]) * i - (s[i]-s[q[head]]);
        while ( head<tail && slope(q[tail-1],q[tail]) >= slope(q[tail],i-m+1) ) tail--;
        q[++tail] = i-m+1;
        if (i>=maxT) ans = min(ans,f[i]);
    }
}
int main(void)
{
    input();
    init();
    dp();
    printf("%lld\n",ans);
    return 0;
}
总结#
其实本题中的这一些问题就对应了斜率优化题中可能会出现的种种问题，以下我们对斜率优化总结一下：

解题基本思路：

1.1. 写出状态转移方程，看看能不能用斜率优化

2.2. 如果可以斜率优化，将方程改写为直线方程的形式，先用数形结合法尝试做一下

3.3. 如果可行，尝试证明一下决策单调性，用代数法推一推

4.4. 看看是什么类型的斜率优化：取minmin就维护下凸壳，取maxmax就维护上凸壳

5.5. 注意一下i,ji,j乘积项的符号，如果是负数一般把负号看成是与jj有关项的而非斜率项的，然后在坐标系内重新画一下图，看看维护的到底是什么

6.6.最后看单调性会不会出问题，如果决策单调性出问题就用二分答案，如果凸壳不能单调维护就用cdqcdq分治

常见问题：

1.1. 决策变量有取值范围，在推入队列的时候改为推入可取的决策变量

2.2. dpdp数组有部分初值无法用斜率优化转移得到(决策变量被取值范围限制)，暴力先转移初值

3.3. 斜率会出现整数被00除，特判返回+∞+∞或−∞−∞

4.4. 计算斜率可能会出锅，slopeslope函数尽量公式化：数值大的下标为yy，数值小的下标为xx，计算时用val(y)−val(x)val(y)−val(x)

5.5. 单调队列要注意：必须在队列内有至少两个元素才能删除队首或队尾

6.6. 浮点数运算很容易出问题，计算斜率或比较大小时记得转为doubledouble类型

7.7. 精度可能会出问题，适当时计算斜率的除法要转为乘法

8.8. 考虑单调队列内是否要存一个转移初值(如00)

9.9. dpdp数组的初值：+∞+∞或−∞−∞或00，是否要long longlong long，无穷要开够大

10.10. 弹出队首不优元素和队尾不在下凸壳内元素比较斜率时，请将等号加上(<<尽量写成≤≤，>>尽量写成≥≥)

11.11. 写二分，请按模板来：whilewhile循环写l<rl<r，每一次计算slope(q[mid],q[mid+1])slope(q[mid],q[mid+1])，若小于等于斜率关键值，则l=mid+1l=mid+1，反之r=midr=mid，最后返回q[l]q[l]

12.12.写cdqcdq分治，也请按模板来：记得在递归子问题前先左右按编号大小分一下，避免出现fifi转移到fjfj但是j>ij>i的情况
