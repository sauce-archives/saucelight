Sauce Light
===========

### Fritzing Diagram

![Sauce Light Fritzing Diagram](https://raw.githubusercontent.com/saucelabs/saucelight/master/saucelight_bb.png?token=AAQsLfBpeRpzl3bu6J7VKpsjU2jsqBnbks5UlIMTwA%3D%3D)

### Build Trigger Commands (for CI system)

##### Running previously blue build

```
curl https://api.spark.io/v1/devices/[spark-device-id]/setState -d access_token=[access-token] -d args=blue_anime
```

##### Running previously red build

```
curl https://api.spark.io/v1/devices/[spark-device-id]/setState -d access_token=[access-token] -d args=red_anime
```

##### Build passed

```
curl https://api.spark.io/v1/devices/[spark-device-id]/setState -d access_token=[access-token] -d args=blue
```

##### Build failed

```
curl https://api.spark.io/v1/devices/[spark-device-id]/setState -d access_token=[access-token] -d args=red
```
