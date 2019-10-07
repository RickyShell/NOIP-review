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
