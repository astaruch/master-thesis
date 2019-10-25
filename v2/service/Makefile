prepare-images:
	docker-compose build thesis-deps
	docker-compose build thesis-runtime

build-travis:
	docker pull astaruch/thesis-deps || true
	docker-compose build thesis-builder
	docker pull astaruch/thesis-runtime || true
	docker-compose build thesis-app
	docker-compose build thesis-tests

build:
	docker-compose build thesis-builder
	docker-compose build thesis-app
	docker-compose build thesis-tests

run:
	docker run --rm --name thesis-phish-app thesis-app

kill:
	docker container stop thesis-phish-app

test:
	docker run --rm --name thesis-phish-app thesis-tests

push-images:
	docker tag thesis-deps astaruch/thesis-deps
	docker push astaruch/thesis-deps
	docker tag thesis-runtime astaruch/thesis-runtime
	docker push astaruch/thesis-runtime
